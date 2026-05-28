#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDef.generated.h"

/**
 * Data-driven definition of an inventory item. Authored as a DataAsset under
 * Content/Data, loaded/indexed via the Asset Manager. See
 * docs/design/inventory-and-economy.md and docs/tech/coding-standards-and-architecture.md.
 */
UENUM(BlueprintType)
enum class EItemCategory : uint8
{
	Material   UMETA(DisplayName = "Material"),
	Tool       UMETA(DisplayName = "Tool"),
	Component  UMETA(DisplayName = "Component"),
	Consumable UMETA(DisplayName = "Consumable"),
	Decor      UMETA(DisplayName = "Decor"),
	KeyOrLore  UMETA(DisplayName = "Key / Lore")
};

UCLASS(BlueprintType)
class THELONGNOON_API UItemDef : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/** Stable id used by recipes, saves and lookups. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FName ItemId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	EItemCategory Category = EItemCategory::Material;

	/** Region tier this item belongs to (1..5). Drives the resource economy gating. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item", meta = (ClampMin = "1", ClampMax = "5"))
	int32 Tier = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item", meta = (ClampMin = "1"))
	int32 MaxStack = 99;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	bool bDroppable = true;

	/** Literacy-aware flavor text. See docs/lore/weapon-and-tool-lore.md. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item", meta = (MultiLine = true))
	FText FlavorText;
};
