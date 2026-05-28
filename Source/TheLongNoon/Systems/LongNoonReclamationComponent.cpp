#include "Systems/LongNoonReclamationComponent.h"
#include "GameFramework/Actor.h"

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

	const AActor* Owner = GetOwner();
	if (!Owner)
	{
		return false;
	}

	// TODO: sphere/line trace ahead within EquippedTool->Reach, find a Bloom target,
	// check target->RequiredVerb against EquippedTool->Verbs, then transition the
	// target to a dormant/settled state (never "dead") and drop materials.
	// See docs/design/reclamation-combat.md for the full target/verb model.

	const float Reach = EquippedTool->Reach;
	UE_LOG(LogTemp, Verbose, TEXT("[Reclamation] Prune attempt with %s (reach %.1f)."),
		*EquippedTool->ToolId.ToString(), Reach);

	return true;
}
