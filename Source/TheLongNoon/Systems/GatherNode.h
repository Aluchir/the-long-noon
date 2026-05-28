#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/Interactable.h"
#include "GatherNode.generated.h"

class UStaticMeshComponent;

/**
 * A harvestable point in the Bloom. Interacting gathers its material into the
 * player's inventory, then it regrows on the activity clock (no true scarcity).
 * See docs/design/resource-economy.md.
 */
UCLASS()
class THELONGNOON_API AGatherNode : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	AGatherNode();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gather")
	FName ItemId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gather", meta = (ClampMin = "1"))
	int32 Quantity = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gather")
	float RegrowSeconds = 30.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gather")
	bool bAvailable = true;

	// IInteractable
	virtual void OnInteract_Implementation(AActor* Interactor) override;
	virtual FText GetInteractPrompt_Implementation() const override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gather")
	TObjectPtr<UStaticMeshComponent> Mesh;

	void Regrow();

	FTimerHandle RegrowTimer;
};
