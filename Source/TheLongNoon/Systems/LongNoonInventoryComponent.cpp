#include "Systems/LongNoonInventoryComponent.h"

void ULongNoonInventoryComponent::AddItem(FName ItemId, int32 Count)
{
	if (ItemId.IsNone() || Count <= 0)
	{
		return;
	}

	int32& Current = Stacks.FindOrAdd(ItemId);
	Current += Count;
	OnInventoryChanged.Broadcast();
}

bool ULongNoonInventoryComponent::RemoveItem(FName ItemId, int32 Count)
{
	if (ItemId.IsNone() || Count <= 0)
	{
		return false;
	}

	int32* Current = Stacks.Find(ItemId);
	if (!Current || *Current < Count)
	{
		return false;
	}

	*Current -= Count;
	if (*Current <= 0)
	{
		Stacks.Remove(ItemId);
	}

	OnInventoryChanged.Broadcast();
	return true;
}

int32 ULongNoonInventoryComponent::GetCount(FName ItemId) const
{
	const int32* Current = Stacks.Find(ItemId);
	return Current ? *Current : 0;
}
