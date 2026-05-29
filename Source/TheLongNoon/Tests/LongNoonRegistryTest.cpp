#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Systems/LongNoonDataRegistry.h"
#include "Data/ToolDef.h"
#include "Data/LongNoonRowTypes.h"

#if WITH_AUTOMATION_TESTS

// Unit tests for the pure static parsers on ULongNoonDataRegistry. These need no
// UWorld, no GameInstance and no loaded DataTables, so they run fast in the build
// pipeline's test step. In UE 5.7 EAutomationTestFlags is a scoped enum; the
// application context mask is the standalone constexpr EAutomationTestFlags_ApplicationContextMask,
// while the filter is the scoped value EAutomationTestFlags::ProductFilter.

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FLongNoonParseRecipeInputsTest,
	"TheLongNoon.DataRegistry.ParseRecipeInputs",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FLongNoonParseRecipeInputsTest::RunTest(const FString& Parameters)
{
	// Well-formed pair list yields one entry per pair with matching id and quantity.
	{
		const TArray<FParsedRecipeInput> Parsed = ULongNoonDataRegistry::ParseRecipeInputs(TEXT("mat_stillsteel:2;comp_fitting:1"));
		TestEqual(TEXT("Two well-formed pairs yield two entries"), Parsed.Num(), 2);
		if (Parsed.Num() == 2)
		{
			TestEqual(TEXT("First entry ItemId"), Parsed[0].ItemId.ToString(), FString(TEXT("mat_stillsteel")));
			TestEqual(TEXT("First entry Quantity"), Parsed[0].Quantity, 2);
			TestEqual(TEXT("Second entry ItemId"), Parsed[1].ItemId.ToString(), FString(TEXT("comp_fitting")));
			TestEqual(TEXT("Second entry Quantity"), Parsed[1].Quantity, 1);
		}
	}

	// A zero (or otherwise non-positive) quantity is clamped up to a minimum of 1.
	{
		const TArray<FParsedRecipeInput> Parsed = ULongNoonDataRegistry::ParseRecipeInputs(TEXT("mat_x:0"));
		TestEqual(TEXT("Single malformed-quantity pair yields one entry"), Parsed.Num(), 1);
		if (Parsed.Num() == 1)
		{
			TestEqual(TEXT("Malformed-quantity ItemId"), Parsed[0].ItemId.ToString(), FString(TEXT("mat_x")));
			TestEqual(TEXT("Zero quantity clamps to one"), Parsed[0].Quantity, 1);
		}
	}

	// Empty input yields no entries.
	{
		const TArray<FParsedRecipeInput> Parsed = ULongNoonDataRegistry::ParseRecipeInputs(FString());
		TestEqual(TEXT("Empty string yields zero entries"), Parsed.Num(), 0);
	}

	// Trailing and doubled separators are tolerated: only real pairs survive.
	{
		const TArray<FParsedRecipeInput> Parsed = ULongNoonDataRegistry::ParseRecipeInputs(TEXT("mat_stillsteel:2;;comp_fitting:1;"));
		TestEqual(TEXT("Extra and trailing semicolons are ignored"), Parsed.Num(), 2);
		if (Parsed.Num() == 2)
		{
			TestEqual(TEXT("Tolerant-parse first ItemId"), Parsed[0].ItemId.ToString(), FString(TEXT("mat_stillsteel")));
			TestEqual(TEXT("Tolerant-parse second ItemId"), Parsed[1].ItemId.ToString(), FString(TEXT("comp_fitting")));
		}
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FLongNoonParseVerbsTest,
	"TheLongNoon.DataRegistry.ParseVerbs",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FLongNoonParseVerbsTest::RunTest(const FString& Parameters)
{
	// Two valid verbs parse in order.
	{
		const TArray<EReclamationVerb> Parsed = ULongNoonDataRegistry::ParseVerbs(TEXT("Seal|Prune"));
		TestEqual(TEXT("Two valid verbs yield two entries"), Parsed.Num(), 2);
		if (Parsed.Num() == 2)
		{
			TestTrue(TEXT("First verb is Seal"), Parsed[0] == EReclamationVerb::Seal);
			TestTrue(TEXT("Second verb is Prune"), Parsed[1] == EReclamationVerb::Prune);
		}
	}

	// An unknown token in the middle is skipped, leaving the valid verbs.
	{
		const TArray<EReclamationVerb> Parsed = ULongNoonDataRegistry::ParseVerbs(TEXT("Prune|Bogus|Quiet"));
		TestEqual(TEXT("Unknown token is skipped, two verbs remain"), Parsed.Num(), 2);
		if (Parsed.Num() == 2)
		{
			TestTrue(TEXT("First surviving verb is Prune"), Parsed[0] == EReclamationVerb::Prune);
			TestTrue(TEXT("Second surviving verb is Quiet"), Parsed[1] == EReclamationVerb::Quiet);
		}
	}

	// Empty input yields no verbs.
	{
		const TArray<EReclamationVerb> Parsed = ULongNoonDataRegistry::ParseVerbs(FString());
		TestEqual(TEXT("Empty string yields zero verbs"), Parsed.Num(), 0);
	}

	return true;
}

#endif // WITH_AUTOMATION_TESTS
