#include "Systems/LongNoonQuestBoard.h"
#include "Systems/LongNoonQuestSubsystem.h"
#include "Components/StaticMeshComponent.h"
#include "Core/LongNoonLog.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"

ALongNoonQuestBoard::ALongNoonQuestBoard()
{
	PrimaryActorTick.bCanEverTick = false;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoardMesh"));
	SetRootComponent(Mesh);
}

void ALongNoonQuestBoard::BeginPlay()
{
	Super::BeginPlay();

	if (!bAutoStart || Objectives.Num() == 0)
	{
		return;
	}

	if (const UWorld* World = GetWorld())
	{
		if (UGameInstance* GI = World->GetGameInstance())
		{
			if (ULongNoonQuestSubsystem* Quests = GI->GetSubsystem<ULongNoonQuestSubsystem>())
			{
				Quests->StartQuest(Objectives);
				UE_LOG(LogLongNoon, Log, TEXT("[QuestBoard] Seeded quest with %d objectives."), Objectives.Num());
			}
		}
	}
}
