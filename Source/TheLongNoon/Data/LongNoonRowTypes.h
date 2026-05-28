#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Data/ItemDef.h"
#include "Data/ToolDef.h"
#include "Data/LoreFragmentDef.h"
#include "LongNoonRowTypes.generated.h"

/**
 * DataTable row structs that mirror the CSVs in Content/Data exactly (column names
 * match property names) so the CSVs import directly as DataTables. The registry
 * (ULongNoonDataRegistry) reads these and parses the packed string columns
 * (verb lists, recipe inputs). See docs/tech/data-authoring-guide.md.
 * Row name (the CSV "Name" column) is the stable id.
 */

USTRUCT(BlueprintType)
struct FItemRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly) FText DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) EItemCategory Category = EItemCategory::Material;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 Tier = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 MaxStack = 99;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) bool bDroppable = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FText FlavorText;
};

USTRUCT(BlueprintType)
struct FToolRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly) FText DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 Tier = 1;
	/** Pipe-separated verbs, e.g. "Seal|Prune". Parsed by the registry into EReclamationVerb. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString Verbs;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float Reach = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float StillDurationSeconds = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float SettlePower = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float StaminaCost = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 MaxDurability = 100;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FText FlavorText;
};

USTRUCT(BlueprintType)
struct FRecipeRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly) FName RequiredStation;
	/** "itemId:qty;itemId:qty" pairs. Parsed by the registry. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString Inputs;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FName Output;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 OutputQuantity = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float StaminaCost = 0.0f;
};

USTRUCT(BlueprintType)
struct FRegionRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly) FText DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 RegionTier = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 RequiredToolTier = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FName RequiredTraversal;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FName RequiredGateBuild;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float ForgettingAmount = 0.0f;
};

USTRUCT(BlueprintType)
struct FLoreFragmentRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly) ELoreFragmentType Type = ELoreFragmentType::Observation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FName Thread;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 RegionTier = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 RequiredLiteracyTier = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FText GlyphText;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FText TranslatedText;
};

/** A parsed recipe input (registry output, not a DataTable column). */
USTRUCT(BlueprintType)
struct FParsedRecipeInput
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly) FName ItemId;
	UPROPERTY(BlueprintReadOnly) int32 Quantity = 1;
};
