// Focus-independent automation tests for core component logic (no input, no world
// needed). Covers inventory stack math and the Tend stamina meter clamping.
#include "CoreMinimal.h"
#if WITH_AUTOMATION_TESTS
#include "Misc/AutomationTest.h"
#include "Systems/LongNoonInventoryComponent.h"
#include "Systems/LongNoonTendComponent.h"

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
#endif // WITH_AUTOMATION_TESTS
