#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LongNoonInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChanged);

/**
 * Closed-economy inventory: a stack count per item id. No currency; materials are
 * the economy. Storage networks (home/outpost) link to this for craft-from-storage.
 * See docs/design/inventory-and-economy.md.
 */
UCLASS(ClassGroup = (LongNoon), meta = (BlueprintSpawnableComponent))
class THELONGNOON_API ULongNoonInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryChanged OnInventoryChanged;

	/** Add Count of an item id. */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItem(FName ItemId, int32 Count = 1);

	/** Remove up to Count; returns true if the full amount was removed. */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItem(FName ItemId, int32 Count = 1);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	int32 GetCount(FName ItemId) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	bool HasItem(FName ItemId, int32 Count = 1) const { return GetCount(ItemId) >= Count; }

	/** Save/restore hooks for the SaveService. */
	const TMap<FName, int32>& GetStacks() const { return Stacks; }
	void RestoreStacks(const TMap<FName, int32>& InStacks) { Stacks = InStacks; OnInventoryChanged.Broadcast(); }

protected:
	/** ItemId -> stack count. Saved/restored by the SaveService. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TMap<FName, int32> Stacks;
};
