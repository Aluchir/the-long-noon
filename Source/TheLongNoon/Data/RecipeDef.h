#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RecipeDef.generated.h"

class UItemDef;

/** One material input for a recipe. */
USTRUCT(BlueprintType)
struct FRecipeInput
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe")
	TSoftObjectPtr<UItemDef> Item;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe", meta = (ClampMin = "1"))
	int32 Quantity = 1;
};

/**
 * A craftable recipe. Closed material economy: inputs are tiered region materials,
 * output is a tool/component/consumable/decor. See docs/design/crafting.md.
 */
UCLASS(BlueprintType)
class THELONGNOON_API URecipeDef : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe")
	FName RecipeId;

	/** Station tier/type required to craft this (basic bench, forge, etc.). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe")
	FName RequiredStation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe")
	TArray<FRecipeInput> Inputs;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe")
	TSoftObjectPtr<UItemDef> Output;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe", meta = (ClampMin = "1"))
	int32 OutputQuantity = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe")
	float StaminaCost = 0.0f;
};
