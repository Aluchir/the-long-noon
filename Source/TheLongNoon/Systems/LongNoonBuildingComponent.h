#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LongNoonBuildingComponent.generated.h"

/**
 * Building: place home/outpost pieces and complete the gate-builds that open
 * regions. Shares a placement model with crafting stations. Refund-friendly so
 * players experiment freely. See docs/design/building.md.
 */
UCLASS(ClassGroup = (LongNoon), meta = (BlueprintSpawnableComponent))
class THELONGNOON_API ULongNoonBuildingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/**
	 * Place a build piece by id at a world transform. Stub: validates id and logs;
	 * full version consumes components, snaps, and spawns the placed actor.
	 */
	UFUNCTION(BlueprintCallable, Category = "Building")
	bool PlacePiece(FName PieceId, const FTransform& Where);

	/**
	 * Mark a region gate-build complete (the Tinker's bridge/clearing/seal). Notifies
	 * the region subsystem so the matching region can be entered.
	 */
	UFUNCTION(BlueprintCallable, Category = "Building")
	void CompleteGateBuild(FName GateBuildId);
};
