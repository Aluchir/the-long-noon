#include "Systems/LongNoonReclamationComponent.h"
#include "Systems/BloomActor.h"
#include "Systems/LongNoonInventoryComponent.h"
#include "Systems/LongNoonTendComponent.h"
#include "Systems/LongNoonDataRegistry.h"
#include "Core/LongNoonLog.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"

void ULongNoonReclamationComponent::EquipTool(UToolDef* Tool)
{
	if (!Tool)
	{
		Equipped = FEquippedTool();
		OnEquippedToolChanged.Broadcast(NAME_None, 0, 0);
		return;
	}

	Equipped.ToolId = Tool->ToolId;
	Equipped.Verbs = Tool->Verbs;
	Equipped.Reach = Tool->Reach;
	Equipped.StaminaCost = Tool->StaminaCost;
	Equipped.MaxDurability = FMath::Max(1, Tool->MaxDurability);
	Equipped.CurrentDurability = Equipped.MaxDurability;
	OnEquippedToolChanged.Broadcast(Equipped.ToolId, Equipped.CurrentDurability, Equipped.MaxDurability);
}

bool ULongNoonReclamationComponent::EquipToolById(FName ToolId)
{
	const UWorld* World = GetWorld();
	UGameInstance* GI = World ? World->GetGameInstance() : nullptr;
	ULongNoonDataRegistry* Registry = GI ? GI->GetSubsystem<ULongNoonDataRegistry>() : nullptr;
	if (!Registry)
	{
		return false;
	}

	const FToolRow* Row = Registry->GetTool(ToolId);
	if (!Row)
	{
		UE_LOG(LogLongNoon, Warning, TEXT("[Reclamation] No tool row '%s'."), *ToolId.ToString());
		return false;
	}

	Equipped.ToolId = ToolId;
	Equipped.Verbs = ULongNoonDataRegistry::ParseVerbs(Row->Verbs);
	Equipped.Reach = Row->Reach;
	Equipped.StaminaCost = Row->StaminaCost;
	Equipped.MaxDurability = FMath::Max(1, Row->MaxDurability);
	Equipped.CurrentDurability = Equipped.MaxDurability;
	OnEquippedToolChanged.Broadcast(Equipped.ToolId, Equipped.CurrentDurability, Equipped.MaxDurability);
	return true;
}

bool ULongNoonReclamationComponent::CanPerform(EReclamationVerb Verb) const
{
	return Equipped.IsValid() && !Equipped.IsWorn() && Equipped.Verbs.Contains(Verb);
}

void ULongNoonReclamationComponent::Repair(int32 Amount)
{
	if (!Equipped.IsValid() || Amount <= 0)
	{
		return;
	}
	Equipped.CurrentDurability = FMath::Min(Equipped.MaxDurability, Equipped.CurrentDurability + Amount);
	OnEquippedToolChanged.Broadcast(Equipped.ToolId, Equipped.CurrentDurability, Equipped.MaxDurability);
}

bool ULongNoonReclamationComponent::TryReclaimAhead()
{
	if (!Equipped.IsValid())
	{
		return false;
	}
	if (Equipped.IsWorn())
	{
		UE_LOG(LogLongNoon, Verbose, TEXT("[Reclamation] %s needs mending before it can be used again."),
			*Equipped.ToolId.ToString());
		return false;
	}

	AActor* Owner = GetOwner();
	UWorld* World = GetWorld();
	if (!Owner || !World)
	{
		return false;
	}

	// Trace forward from the owner within the tool's reach (reach is in metres -> cm).
	const FVector Start = Owner->GetActorLocation();
	const FVector End = Start + Owner->GetActorForwardVector() * (Equipped.Reach * 100.0f);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Owner);

	if (!World->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		return false;
	}

	ABloomActor* Bloom = Cast<ABloomActor>(Hit.GetActor());
	if (!Bloom)
	{
		return false;
	}

	// Use the target's required verb only if this tool provides it.
	if (!CanPerform(Bloom->RequiredVerb))
	{
		UE_LOG(LogLongNoon, Verbose, TEXT("[Reclamation] %s lacks the verb for this growth."),
			*Equipped.ToolId.ToString());
		return false;
	}

	TMap<FName, int32> Drops;
	if (!Bloom->TryReclaim(Bloom->RequiredVerb, Drops))
	{
		return false;
	}

	if (ULongNoonInventoryComponent* Inv = Owner->FindComponentByClass<ULongNoonInventoryComponent>())
	{
		for (const TPair<FName, int32>& Drop : Drops)
		{
			Inv->AddItem(Drop.Key, Drop.Value);
		}
	}

	// Spend stamina on the Tend component (mirrors crafting; hitting zero only slows, never kills).
	if (Equipped.StaminaCost > 0.0f)
	{
		if (ULongNoonTendComponent* Tend = Owner->FindComponentByClass<ULongNoonTendComponent>())
		{
			Tend->SpendStamina(Equipped.StaminaCost);
		}
	}

	// Wear the tool's durability (the main upkeep pressure; restored with repair kits).
	if (DurabilityCostPerUse > 0)
	{
		Equipped.CurrentDurability = FMath::Max(0, Equipped.CurrentDurability - DurabilityCostPerUse);
		OnEquippedToolChanged.Broadcast(Equipped.ToolId, Equipped.CurrentDurability, Equipped.MaxDurability);
		if (Equipped.IsWorn())
		{
			UE_LOG(LogLongNoon, Log, TEXT("[Reclamation] %s is worn and needs mending."),
				*Equipped.ToolId.ToString());
		}
	}

	return true;
}
