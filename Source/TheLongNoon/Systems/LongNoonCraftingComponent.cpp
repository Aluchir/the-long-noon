#include "Systems/LongNoonCraftingComponent.h"
#include "Systems/LongNoonInventoryComponent.h"
#include "Systems/LongNoonDataRegistry.h"
#include "Systems/LongNoonTendComponent.h"
#include "Core/LongNoonEventSubsystem.h"
#include "Core/LongNoonGameInstance.h"
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

	// Spend stamina on the Tend component if present (mirrors CraftById).
	if (Recipe->StaminaCost > 0.0f)
	{
		if (ULongNoonTendComponent* Tend = GetOwner() ? GetOwner()->FindComponentByClass<ULongNoonTendComponent>() : nullptr)
		{
			Tend->SpendStamina(Recipe->StaminaCost);
		}
	}

	return true;
}

bool ULongNoonCraftingComponent::CraftById(FName RecipeId)
{
	const UWorld* World = GetWorld();
	UGameInstance* GI = World ? World->GetGameInstance() : nullptr;
	ULongNoonDataRegistry* Registry = GI ? GI->GetSubsystem<ULongNoonDataRegistry>() : nullptr;
	ULongNoonInventoryComponent* Inv = GetInventory();
	if (!Registry || !Inv)
	{
		return false;
	}

	const FRecipeRow* Recipe = Registry->GetRecipe(RecipeId);
	if (!Recipe)
	{
		UE_LOG(LogLongNoon, Warning, TEXT("[Crafting] No recipe row '%s'."), *RecipeId.ToString());
		return false;
	}

	if (!Recipe->RequiredStation.IsNone() && Recipe->RequiredStation != CurrentStation)
	{
		return false;
	}

	const TArray<FParsedRecipeInput> Inputs = ULongNoonDataRegistry::ParseRecipeInputs(Recipe->Inputs);
	for (const FParsedRecipeInput& In : Inputs)
	{
		if (!Inv->HasItem(In.ItemId, In.Quantity))
		{
			return false;
		}
	}

	for (const FParsedRecipeInput& In : Inputs)
	{
		Inv->RemoveItem(In.ItemId, In.Quantity);
	}
	Inv->AddItem(Recipe->Output, Recipe->OutputQuantity);

	// If the crafted item is a reclamation tool, raise the player's max tool tier
	// (this is what gates region progression; see ULongNoonRegionSubsystem).
	if (const FToolRow* ToolRow = Registry->GetTool(Recipe->Output))
	{
		if (ULongNoonGameInstance* LNGI = Cast<ULongNoonGameInstance>(GI))
		{
			LNGI->MaxToolTier = FMath::Max(LNGI->MaxToolTier, ToolRow->Tier);
			UE_LOG(LogLongNoon, Log, TEXT("[Crafting] Crafted tool '%s' (tier %d); MaxToolTier now %d."),
				*Recipe->Output.ToString(), ToolRow->Tier, LNGI->MaxToolTier);
		}
	}

	if (ULongNoonEventSubsystem* Events = GI->GetSubsystem<ULongNoonEventSubsystem>())
	{
		Events->BroadcastItemCrafted(Recipe->Output);
	}

	// Spend stamina on the Tend component if present.
	if (Recipe->StaminaCost > 0.0f)
	{
		if (UActorComponent* TendComp = GetOwner() ? GetOwner()->FindComponentByClass(ULongNoonTendComponent::StaticClass()) : nullptr)
		{
			Cast<ULongNoonTendComponent>(TendComp)->SpendStamina(Recipe->StaminaCost);
		}
	}

	return true;
}
