#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/Interactable.h"
#include "LongNoonCraftingStation.generated.h"

class UStaticMeshComponent;

/**
 * A placeable crafting bench. Interacting opens the crafting menu for this station's
 * recipes, wired to the interactor's crafting component. StationId matches the
 * RequiredStation column in Recipes.csv (e.g. "station_basic_bench").
 */
UCLASS()
class THELONGNOON_API ALongNoonCraftingStation : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	ALongNoonCraftingStation();

	/** Station id (matches Recipes.csv RequiredStation). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crafting")
	FName StationId = TEXT("station_basic_bench");

	virtual void OnInteract_Implementation(AActor* Interactor) override;
	virtual FText GetInteractPrompt_Implementation() const override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crafting")
	TObjectPtr<UStaticMeshComponent> Mesh;
};
