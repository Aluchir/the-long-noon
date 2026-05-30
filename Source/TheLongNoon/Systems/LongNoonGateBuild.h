#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/Interactable.h"
#include "LongNoonGateBuild.generated.h"

class UStaticMeshComponent;

/**
 * A buildable region gate in the world (the Tinker's bridge/clearing/seal). Interacting
 * routes through the interactor's building component to mark the gate-build complete
 * (so the matching region can be entered) and optionally completes a quest objective,
 * then the unbuilt marker hides to reveal the finished way. This is the "build" leg of
 * the gather -> craft -> prune -> build loop. See docs/design/building.md.
 */
UCLASS()
class THELONGNOON_API ALongNoonGateBuild : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	ALongNoonGateBuild();

	/** Gate-build id marked complete (matches Regions.csv RequiredGateBuild). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Building")
	FName GateBuildId;

	/** Optional quest objective id completed when this gate is built. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Building")
	FName CompletesObjective;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Building")
	bool bBuilt = false;

	virtual void OnInteract_Implementation(AActor* Interactor) override;
	virtual FText GetInteractPrompt_Implementation() const override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Building")
	TObjectPtr<UStaticMeshComponent> Mesh;
};
