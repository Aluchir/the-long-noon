// Focus-independent automation tests for core component logic (no input, no world
// needed). Covers inventory stack math and the Tend stamina meter clamping.
#include "CoreMinimal.h"
#if WITH_AUTOMATION_TESTS
#include "Misc/AutomationTest.h"
#include "Systems/LongNoonInventoryComponent.h"
#include "Systems/LongNoonTendComponent.h"
#include "Systems/LongNoonReclamationComponent.h"
#include "Data/ToolDef.h"

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
#endif // WITH_AUTOMATION_TESTS
