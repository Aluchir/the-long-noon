#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LongNoonRegionSubsystem.generated.h"

class URegionDef;

/**
 * Region gating: progression is descent into truth. Entry is a composite gate
 * (tool tier + traversal capability + completed gate-build). This evaluates
 * whether the player may enter a region and records unlocks.
 * See docs/design/progression-and-gating.md.
 */
UCLASS()
class THELONGNOON_API ULongNoonRegionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/** Composite gate check against current progression + acquired capabilities. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Region")
	bool CanEnterRegion(const URegionDef* Region) const;

	/** Mark a region unlocked and broadcast it. */
	UFUNCTION(BlueprintCallable, Category = "Region")
	void UnlockRegion(const URegionDef* Region);

	/** Player-acquired traversal capabilities (e.g. "Climb", "Seal"). */
	UFUNCTION(BlueprintCallable, Category = "Region")
	void GrantTraversal(FName Capability) { AcquiredTraversals.Add(Capability); }

	/** Completed gate-builds (the Tinker's bridge/clearing/seal). */
	UFUNCTION(BlueprintCallable, Category = "Region")
	void MarkGateBuildComplete(FName GateBuild) { CompletedGateBuilds.Add(GateBuild); }

private:
	UPROPERTY()
	TSet<FName> AcquiredTraversals;

	UPROPERTY()
	TSet<FName> CompletedGateBuilds;
};
