#include "Systems/GatherNode.h"
#include "Systems/LongNoonInventoryComponent.h"
#include "Systems/LongNoonQuestSubsystem.h"
#include "Components/StaticMeshComponent.h"
#include "Core/LongNoonLog.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "UObject/ConstructorHelpers.h"

AGatherNode::AGatherNode()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GatherMesh"));
	SetRootComponent(Mesh);

	// Default gather cue: the CC0 "chop" SFX (Kenney RPG Audio). Editable per instance.
	static ConstructorHelpers::FObjectFinder<USoundBase> ChopSound(TEXT("/Game/ThirdParty/Audio/SFX/chop.chop"));
	if (ChopSound.Succeeded())
	{
		GatherSound = ChopSound.Object;
	}
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

		if (GatherSound)
		{
			UGameplayStatics::PlaySound2D(this, GatherSound);
		}

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
