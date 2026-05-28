#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LongNoonCraftingComponent.generated.h"

class URecipeDef;
class ULongNoonInventoryComponent;

/**
 * Crafting: consume a recipe's material inputs from the owner's inventory and
 * produce the output. Closed economy, station-gated, costs stamina (Tend).
 * See docs/design/crafting.md.
 */
UCLASS(ClassGroup = (LongNoon), meta = (BlueprintSpawnableComponent))
class THELONGNOON_API ULongNoonCraftingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Station the player is currently at (set by the building/station system). None = no station. */
	UPROPERTY(BlueprintReadWrite, Category = "Crafting")
	FName CurrentStation;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Crafting")
	bool CanCraft(const URecipeDef* Recipe) const;

	/** Craft the recipe if inputs and station are satisfied. Returns true on success. */
	UFUNCTION(BlueprintCallable, Category = "Crafting")
	bool Craft(const URecipeDef* Recipe);

	/** Craft by recipe id, resolving the recipe from the data registry (CSV-backed path). */
	UFUNCTION(BlueprintCallable, Category = "Crafting")
	bool CraftById(FName RecipeId);

protected:
	ULongNoonInventoryComponent* GetInventory() const;
};
