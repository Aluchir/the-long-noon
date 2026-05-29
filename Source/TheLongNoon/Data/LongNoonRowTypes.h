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

USTRUCT(BlueprintType)
struct FNpcRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly) FText DisplayName;
	/** Region id the NPC is first met in (matches Regions.csv). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FName HomeRegion;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString Role;
	/** Lore fragment granted on first talk (None = none). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FName GrantsFragmentOnFirstTalk;
	/** Pipe-separated DialogueLines ids, e.g. "tinker_first1|tinker_first2". */
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString FirstTalkLines;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString IdleLines;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString StoryBeatLines;
	/** The single line that, on reflection, guts the player. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FName DaggerLine;
};

USTRUCT(BlueprintType)
struct FDialogueLineRow : public FTableRowBase
{
	GENERATED_BODY()

	/** NPC id this line belongs to (matches NPCs.csv Name). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FName Speaker;
	/** FirstTalk | Idle | StoryBeat | Dagger. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FName Category;
	/** Freeform trigger token (first_talk, on_first_craft, the_offer, ...). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FName Trigger;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FText Text;
};

USTRUCT(BlueprintType)
struct FBuildRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly) FText DisplayName;
	/** "itemId:qty;itemId:qty" pairs, same encoding as recipes. Parse with ParseRecipeInputs. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString Inputs;
	/** Region id this gate-build unlocks (matches Regions.csv, and Regions.RequiredGateBuild references this row). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FName RegionUnlocked;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float StaminaCost = 0.0f;
};

/** A parsed recipe input (registry output, not a DataTable column). */
USTRUCT(BlueprintType)
struct FParsedRecipeInput
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly) FName ItemId;
	UPROPERTY(BlueprintReadOnly) int32 Quantity = 1;
};
