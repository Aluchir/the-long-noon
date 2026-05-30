#include "Systems/LongNoonNpc.h"
#include "Systems/LongNoonDialogueComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Core/LongNoonLog.h"

ALongNoonNpc::ALongNoonNpc()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NpcMesh"));
	SetRootComponent(Mesh);

	Dialogue = CreateDefaultSubobject<ULongNoonDialogueComponent>(TEXT("Dialogue"));
}

void ALongNoonNpc::BeginPlay()
{
	Super::BeginPlay();

	if (Dialogue && !NpcId.IsNone())
	{
		Dialogue->NpcId = NpcId;
		const bool bOk = Dialogue->LoadFromData();
		UE_LOG(LogLongNoon, Log, TEXT("[NPC] %s LoadFromData=%s lines=%d"),
			*NpcId.ToString(), bOk ? TEXT("ok") : TEXT("FAIL"), Dialogue->Lines.Num());
	}
}

void ALongNoonNpc::OnInteract_Implementation(AActor* /*Interactor*/)
{
	if (Dialogue)
	{
		Dialogue->StartDialogue();
		UE_LOG(LogLongNoon, Log, TEXT("[NPC] %s talk started (%d lines)"),
			*NpcId.ToString(), Dialogue->Lines.Num());
	}
}

FText ALongNoonNpc::GetInteractPrompt_Implementation() const
{
	return NSLOCTEXT("LongNoon", "TalkPrompt", "Talk");
}
