#include "Systems/BloomActor.h"
#include "Systems/LongNoonQuestSubsystem.h"
#include "Components/StaticMeshComponent.h"
#include "Core/LongNoonLog.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"

ABloomActor::ABloomActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BloomMesh"));
	SetRootComponent(Mesh);
}

bool ABloomActor::TryReclaim(EReclamationVerb Verb, TMap<FName, int32>& OutDrops)
{
	OutDrops.Reset();

	if (State != EBloomState::Active || Verb != RequiredVerb)
	{
		return false;
	}

	OutDrops = DropTable;

	// Stilling makes it dormant; everything else settles it asleep. Never "dead".
	State = (Verb == EReclamationVerb::Still) ? EBloomState::Dormant : EBloomState::Settled;

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(RegrowTimer, this, &ABloomActor::Regrow, RegrowSeconds, false);
	}

	UE_LOG(LogLongNoon, Verbose, TEXT("[Bloom] Reclaimed tier %d patch -> %d."), BloomTier, (int32)State);

	// Advance the quest if this patch completes an objective (mirrors AGatherNode).
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

	return true;
}

void ABloomActor::Regrow()
{
	State = EBloomState::Active;
	UE_LOG(LogLongNoon, Verbose, TEXT("[Bloom] Patch regrew (the world never stops growing)."));
}
