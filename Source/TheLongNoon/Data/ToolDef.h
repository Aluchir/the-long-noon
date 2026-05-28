#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ToolDef.generated.h"

/**
 * Reclamation / utility tool definition. Tools are non-lethal: they prune, still,
 * settle, seal, or quiet the Bloom. Tiers map 1:1 to regions and gate progression.
 * See docs/design/weapon-and-tool-tiers.md and docs/design/reclamation-combat.md.
 */
UENUM(BlueprintType)
enum class EReclamationVerb : uint8
{
	Prune  UMETA(DisplayName = "Prune"),
	Still  UMETA(DisplayName = "Still"),
	Settle UMETA(DisplayName = "Settle"),
	Seal   UMETA(DisplayName = "Seal"),
	Quiet  UMETA(DisplayName = "Quiet")
};

UCLASS(BlueprintType)
class THELONGNOON_API UToolDef : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tool")
	FName ToolId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tool")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tool", meta = (ClampMin = "1", ClampMax = "5"))
	int32 Tier = 1;

	/** Verbs this tool can perform (a target is handled if the tool provides its required verb). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tool")
	TArray<EReclamationVerb> Verbs;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tool|Stats")
	float Reach = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tool|Stats")
	float StillDurationSeconds = 4.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tool|Stats")
	float SettlePower = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tool|Stats")
	float StaminaCost = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tool|Stats", meta = (ClampMin = "1"))
	int32 MaxDurability = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tool", meta = (MultiLine = true))
	FText FlavorText;
};
