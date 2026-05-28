#include "Systems/LongNoonCraftingComponent.h"
#include "Systems/LongNoonInventoryComponent.h"
#include "Core/LongNoonEventSubsystem.h"
#include "Core/LongNoonLog.h"
#include "Data/RecipeDef.h"
#include "Data/ItemDef.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"

ULongNoonInventoryComponent* ULongNoonCraftingComponent::GetInventory() const
{
	const AActor* Owner = GetOwner();
	return Owner ? Owner->FindComponentByClass<ULongNoonInventoryComponent>() : nullptr;
}

bool ULongNoonCraftingComponent::CanCraft(const URecipeDef* Recipe) const
{
	if (!Recipe)
	{
		return false;
	}
	if (!Recipe->RequiredStation.IsNone() && Recipe->RequiredStation != CurrentStation)
	{
		return false;
	}

	const ULongNoonInventoryComponent* Inv = GetInventory();
	if (!Inv)
	{
		return false;
	}

	for (const FRecipeInput& In : Recipe->Inputs)
	{
		const UItemDef* Item = In.Item.LoadSynchronous();
		if (!Item || !Inv->HasItem(Item->ItemId, In.Quantity))
		{
			return false;
		}
	}
	return true;
}

bool ULongNoonCraftingComponent::Craft(const URecipeDef* Recipe)
{
	if (!CanCraft(Recipe))
	{
		return false;
	}

	ULongNoonInventoryComponent* Inv = GetInventory();
	for (const FRecipeInput& In : Recipe->Inputs)
	{
		const UItemDef* Item = In.Item.LoadSynchronous();
		Inv->RemoveItem(Item->ItemId, In.Quantity);
	}

	const UItemDef* Output = Recipe->Output.LoadSynchronous();
	if (!Output)
	{
		UE_LOG(LogLongNoon, Warning, TEXT("[Crafting] Recipe %s has no output."), *Recipe->RecipeId.ToString());
		return false;
	}

	Inv->AddItem(Output->ItemId, Recipe->OutputQuantity);

	if (const UWorld* World = GetWorld())
	{
		if (UGameInstance* GI = World->GetGameInstance())
		{
			if (ULongNoonEventSubsystem* Events = GI->GetSubsystem<ULongNoonEventSubsystem>())
			{
				Events->BroadcastItemCrafted(Output->ItemId);
			}
		}
	}

	// TODO: spend Recipe->StaminaCost on the Tend component.
	return true;
}
