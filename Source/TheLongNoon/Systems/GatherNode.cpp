#include "Systems/GatherNode.h"
#include "Systems/LongNoonInventoryComponent.h"
#include "Systems/LongNoonQuestSubsystem.h"
#include "Components/StaticMeshComponent.h"
#include "Core/LongNoonLog.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"

AGatherNode::AGatherNode()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GatherMesh"));
	SetRootComponent(Mesh);
}

void AGatherNode::OnInteract_Implementation(AActor* Interactor)
{
	if (!bAvailable || ItemId.IsNone() || !Interactor)
	{
		return;
	}

	// Find an inventory on whoever interacted (no hard dependency on the character class).
	if (ULongNoonInventoryComponent* Inv = Interactor->FindComponentByClass<ULongNoonInventoryComponent>())
	{
		Inv->AddItem(ItemId, Quantity);
		bAvailable = false;

		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimer(RegrowTimer, this, &AGatherNode::Regrow, RegrowSeconds, false);
		}

		UE_LOG(LogLongNoon, Log, TEXT("[Gather] %d x %s gathered."), Quantity, *ItemId.ToString());

		// Advance the quest if this node completes an objective.
		if (!CompletesObjective.IsNone())
		{
			if (const UWorld* World = GetWorld())
			{
				if (UGameInstance* GI = World->GetGameInstance())
				{
					if (ULongNoonQuestSubsystem* Quests = GI->GetSubsystem<ULongNoonQuestSubsystem>())
					{
						Quests->CompleteObjective(CompletesObjective);
					}
				}
			}
		}
	}
}

FText AGatherNode::GetInteractPrompt_Implementation() const
{
	return NSLOCTEXT("LongNoon", "GatherPrompt", "Gather");
}

void AGatherNode::Regrow()
{
	bAvailable = true;
}
