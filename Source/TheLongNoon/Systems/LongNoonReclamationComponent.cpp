#include "Systems/LongNoonReclamationComponent.h"
#include "Systems/BloomActor.h"
#include "Systems/LongNoonInventoryComponent.h"
#include "Core/LongNoonLog.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

bool ULongNoonReclamationComponent::CanPerform(EReclamationVerb Verb) const
{
	return EquippedTool && EquippedTool->Verbs.Contains(Verb);
}

bool ULongNoonReclamationComponent::TryReclaimAhead()
{
	if (!EquippedTool)
	{
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
	const FVector End = Start + Owner->GetActorForwardVector() * (EquippedTool->Reach * 100.0f);

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
			*EquippedTool->ToolId.ToString());
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

	// TODO: spend EquippedTool->StaminaCost on the Tend component; apply tool durability.
	return true;
}
