// Focus-independent automation tests for core component logic (no input, no world
// needed). Covers inventory stack math and the Tend stamina meter clamping.
#include "CoreMinimal.h"
#if WITH_AUTOMATION_TESTS
#include "Misc/AutomationTest.h"
#include "Systems/LongNoonInventoryComponent.h"
#include "Systems/LongNoonTendComponent.h"
#include "Systems/LongNoonReclamationComponent.h"
#include "Systems/LongNoonSaveGame.h"
#include "Systems/LongNoonRegionSubsystem.h"
#include "Systems/LongNoonCodexSubsystem.h"
#include "Systems/LongNoonEndings.h"
#include "Data/ToolDef.h"
#include "Kismet/GameplayStatics.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLongNoonInventoryTest, "TheLongNoon.Systems.Inventory",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FLongNoonInventoryTest::RunTest(const FString& Parameters)
{
	ULongNoonInventoryComponent* Inv = NewObject<ULongNoonInventoryComponent>();
	TestNotNull(TEXT("inventory created"), Inv);

	Inv->AddItem(TEXT("mat_sunmoss"), 3);
	TestEqual(TEXT("add 3"), Inv->GetCount(TEXT("mat_sunmoss")), 3);

	Inv->AddItem(TEXT("mat_sunmoss"), 2);
	TestEqual(TEXT("add 2 more -> 5"), Inv->GetCount(TEXT("mat_sunmoss")), 5);

	TestTrue(TEXT("has 5"), Inv->HasItem(TEXT("mat_sunmoss"), 5));
	TestFalse(TEXT("does not have 6"), Inv->HasItem(TEXT("mat_sunmoss"), 6));

	const bool bRemovedAll = Inv->RemoveItem(TEXT("mat_sunmoss"), 2);
	TestTrue(TEXT("remove 2 succeeds"), bRemovedAll);
	TestEqual(TEXT("count 3 after remove"), Inv->GetCount(TEXT("mat_sunmoss")), 3);

	const bool bRemovedTooMany = Inv->RemoveItem(TEXT("mat_sunmoss"), 99);
	TestFalse(TEXT("removing more than held returns false"), bRemovedTooMany);
	TestEqual(TEXT("unknown item count is 0"), Inv->GetCount(TEXT("mat_none")), 0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLongNoonTendTest, "TheLongNoon.Systems.TendStamina",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FLongNoonTendTest::RunTest(const FString& Parameters)
{
	ULongNoonTendComponent* Tend = NewObject<ULongNoonTendComponent>();
	TestNotNull(TEXT("tend created"), Tend);
	TestEqual(TEXT("starts full"), Tend->Stamina, Tend->MaxStamina);

	const bool bEnough = Tend->SpendStamina(30.0f);
	TestTrue(TEXT("spend 30 had enough"), bEnough);
	TestEqual(TEXT("stamina 70"), Tend->Stamina, Tend->MaxStamina - 30.0f);
	TestTrue(TEXT("has 70"), Tend->HasStamina(70.0f));

	const bool bOverdraw = Tend->SpendStamina(1000.0f);
	TestFalse(TEXT("overdraw reports not enough"), bOverdraw);
	TestEqual(TEXT("stamina clamps at 0, never negative"), Tend->Stamina, 0.0f);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLongNoonReclamationTest, "TheLongNoon.Systems.Reclamation",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FLongNoonReclamationTest::RunTest(const FString& Parameters)
{
	ULongNoonReclamationComponent* Recl = NewObject<ULongNoonReclamationComponent>();
	TestNotNull(TEXT("reclamation created"), Recl);
	TestFalse(TEXT("no tool initially"), Recl->HasTool());

	UToolDef* Tool = NewObject<UToolDef>();
	Tool->ToolId = TEXT("tool_test_blade");
	Tool->Verbs.Add(EReclamationVerb::Prune);
	Tool->MaxDurability = 80;
	Recl->EquipTool(Tool);

	TestTrue(TEXT("has tool after equip"), Recl->HasTool());
	TestTrue(TEXT("can Prune (tool provides it)"), Recl->CanPerform(EReclamationVerb::Prune));
	TestFalse(TEXT("cannot Seal (tool lacks it)"), Recl->CanPerform(EReclamationVerb::Seal));

	const FEquippedTool Eq = Recl->GetEquippedTool();
	TestEqual(TEXT("equipped id matches"), Eq.ToolId, FName(TEXT("tool_test_blade")));
	TestEqual(TEXT("durability starts at max"), Eq.CurrentDurability, 80);

	Recl->EquipTool(nullptr);
	TestFalse(TEXT("unequip clears tool"), Recl->HasTool());
	TestFalse(TEXT("cannot perform with no tool"), Recl->CanPerform(EReclamationVerb::Prune));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLongNoonSaveTest, "TheLongNoon.Systems.SaveRoundTrip",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FLongNoonSaveTest::RunTest(const FString& Parameters)
{
	const FString Slot = TEXT("TLN_AutomationTest");
	ULongNoonSaveGame* Save = Cast<ULongNoonSaveGame>(
		UGameplayStatics::CreateSaveGameObject(ULongNoonSaveGame::StaticClass()));
	TestNotNull(TEXT("save object created"), Save);
	if (!Save) { return false; }

	Save->ScriptLiteracyTier = 3;
	Save->MaxToolTier = 2;
	Save->bRemembererFound = true;
	Save->InventoryStacks.Add(TEXT("mat_sunmoss"), 5);
	Save->FoundFragments.Add(TEXT("frag_sunhollow_monument"));
	Save->UnlockedRegions.Add(TEXT("region_overgrowth"));
	Save->CurrentRegionId = TEXT("region_sunhollow");

	TestTrue(TEXT("saved to slot"), UGameplayStatics::SaveGameToSlot(Save, Slot, 0));
	TestTrue(TEXT("slot exists"), UGameplayStatics::DoesSaveGameExist(Slot, 0));

	ULongNoonSaveGame* Loaded = Cast<ULongNoonSaveGame>(UGameplayStatics::LoadGameFromSlot(Slot, 0));
	TestNotNull(TEXT("loaded back"), Loaded);
	if (Loaded)
	{
		TestEqual(TEXT("literacy round-trips"), Loaded->ScriptLiteracyTier, 3);
		TestEqual(TEXT("tool tier round-trips"), Loaded->MaxToolTier, 2);
		TestTrue(TEXT("rememberer flag round-trips"), Loaded->bRemembererFound);
		TestEqual(TEXT("inventory stack round-trips"), Loaded->InventoryStacks.FindRef(TEXT("mat_sunmoss")), 5);
		TestTrue(TEXT("found fragment round-trips"), Loaded->FoundFragments.Contains(TEXT("frag_sunhollow_monument")));
		TestTrue(TEXT("unlocked region round-trips"), Loaded->UnlockedRegions.Contains(TEXT("region_overgrowth")));
		TestEqual(TEXT("current region round-trips"), Loaded->CurrentRegionId, FName(TEXT("region_sunhollow")));
	}
	UGameplayStatics::DeleteGameInSlot(Slot, 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLongNoonRegionGateTest, "TheLongNoon.Systems.RegionGate",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FLongNoonRegionGateTest::RunTest(const FString& Parameters)
{
	TSet<FName> NoCaps;
	TSet<FName> WithSeal; WithSeal.Add(TEXT("Seal"));
	TSet<FName> WithChoke; WithChoke.Add(TEXT("gate_the_choke"));

	// Sunhollow-like: req tier 1, no traversal, no build -> enter with tier 1.
	TestTrue(TEXT("open region with tier 1"),
		ULongNoonRegionSubsystem::CanEnter(1, NAME_None, NAME_None, 1, NoCaps, NoCaps));

	// Tool tier too low blocks.
	TestFalse(TEXT("tier 2 region blocked at tier 1"),
		ULongNoonRegionSubsystem::CanEnter(2, NAME_None, NAME_None, 1, NoCaps, NoCaps));

	// Missing gate-build blocks even with the tool tier.
	TestFalse(TEXT("missing gate-build blocks"),
		ULongNoonRegionSubsystem::CanEnter(1, NAME_None, TEXT("gate_the_choke"), 2, NoCaps, NoCaps));
	TestTrue(TEXT("gate-build present passes"),
		ULongNoonRegionSubsystem::CanEnter(1, NAME_None, TEXT("gate_the_choke"), 2, NoCaps, WithChoke));

	// Missing traversal (Hush needs Seal) blocks; granting Seal passes.
	TestFalse(TEXT("missing Seal traversal blocks"),
		ULongNoonRegionSubsystem::CanEnter(3, TEXT("Seal"), NAME_None, 3, NoCaps, NoCaps));
	TestTrue(TEXT("Seal traversal granted passes"),
		ULongNoonRegionSubsystem::CanEnter(3, TEXT("Seal"), NAME_None, 3, WithSeal, NoCaps));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLongNoonLiteracyTest, "TheLongNoon.Systems.LiteracyReveal",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FLongNoonLiteracyTest::RunTest(const FString& Parameters)
{
	// Tier-0 fragments are always readable.
	TestTrue(TEXT("tier-0 readable at literacy 0"), ULongNoonCodexSubsystem::IsLiteracyEnough(0, 0));
	// A tier-2 fragment is unreadable until literacy reaches 2, then stays readable.
	TestFalse(TEXT("tier-2 unreadable at literacy 0"), ULongNoonCodexSubsystem::IsLiteracyEnough(2, 0));
	TestFalse(TEXT("tier-2 unreadable at literacy 1"), ULongNoonCodexSubsystem::IsLiteracyEnough(2, 1));
	TestTrue(TEXT("tier-2 readable at literacy 2"), ULongNoonCodexSubsystem::IsLiteracyEnough(2, 2));
	TestTrue(TEXT("tier-2 readable at literacy 4"), ULongNoonCodexSubsystem::IsLiteracyEnough(2, 4));
	// The Gardener's word (tier 4) only at full literacy.
	TestFalse(TEXT("tier-4 unreadable at literacy 3"), ULongNoonCodexSubsystem::IsLiteracyEnough(4, 3));
	TestTrue(TEXT("tier-4 readable at literacy 4"), ULongNoonCodexSubsystem::IsLiteracyEnough(4, 4));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLongNoonEndingsTest, "TheLongNoon.Systems.Endings",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FLongNoonEndingsTest::RunTest(const FString& Parameters)
{
	TestEqual(TEXT("give it back -> Gift Returned"),
		(int32)ULongNoonEndingLibrary::ResolveEnding(ELongNoonChoice::GiveItBack, false),
		(int32)ELongNoonEnding::GiftReturned);
	TestEqual(TEXT("keep the noon -> Long Noon Continues"),
		(int32)ULongNoonEndingLibrary::ResolveEnding(ELongNoonChoice::KeepTheNoon, false),
		(int32)ELongNoonEnding::LongNoonContinues);
	TestEqual(TEXT("third way without Rememberer -> None"),
		(int32)ULongNoonEndingLibrary::ResolveEnding(ELongNoonChoice::TheThirdWay, false),
		(int32)ELongNoonEnding::None);
	TestEqual(TEXT("third way with Rememberer -> Third Way"),
		(int32)ULongNoonEndingLibrary::ResolveEnding(ELongNoonChoice::TheThirdWay, true),
		(int32)ELongNoonEnding::ThirdWay);
	TestFalse(TEXT("third way unavailable without Rememberer"), ULongNoonEndingLibrary::IsThirdWayAvailable(false));
	TestTrue(TEXT("third way available with Rememberer"), ULongNoonEndingLibrary::IsThirdWayAvailable(true));
	return true;
}
#endif // WITH_AUTOMATION_TESTS
