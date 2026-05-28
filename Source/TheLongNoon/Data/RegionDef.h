#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RegionDef.generated.h"

/**
 * A region and its entry gate. Gating is a composite: a tool tier + a traversal
 * capability + a completed gate-build. Progression is descent into truth.
 * See docs/design/progression-and-gating.md and the docs/lore/region-*.md files.
 */
UCLASS(BlueprintType)
class THELONGNOON_API URegionDef : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Region")
	FName RegionId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Region")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Region", meta = (ClampMin = "1", ClampMax = "5"))
	int32 RegionTier = 1;

	/** Map asset for this region (Content/Maps). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Region", meta = (AllowedTypes = "World"))
	TSoftObjectPtr<UWorld> Level;

	/** Minimum reclamation tool tier required to enter. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Region|Gate", meta = (ClampMin = "1", ClampMax = "5"))
	int32 RequiredToolTier = 1;

	/** Named traversal capability required (e.g. "Climb", "Seal"). Empty = none. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Region|Gate")
	FName RequiredTraversal;

	/** Gate-build that must be completed to open the path here (the Tinker's bridge/clearing/seal). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Region|Gate")
	FName RequiredGateBuild;

	/** The "wrongness" of the golden hour here (0 = warmest Sunhollow, 1 = most drained Hush). Art dial. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Region|Mood", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float ForgettingAmount = 0.0f;
};
