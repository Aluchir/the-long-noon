#include "Systems/LongNoonLoreFragment.h"
#include "Systems/LongNoonCodexSubsystem.h"
#include "Components/StaticMeshComponent.h"
#include "Core/LongNoonLog.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"

ALongNoonLoreFragment::ALongNoonLoreFragment()
{
	PrimaryActorTick.bCanEverTick = false;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FragmentMesh"));
	SetRootComponent(Mesh);
}

void ALongNoonLoreFragment::OnInteract_Implementation(AActor* /*Interactor*/)
{
	if (bFound || FragmentId.IsNone())
	{
		return;
	}

	if (const UWorld* World = GetWorld())
	{
		if (UGameInstance* GI = World->GetGameInstance())
		{
			if (ULongNoonCodexSubsystem* Codex = GI->GetSubsystem<ULongNoonCodexSubsystem>())
			{
				Codex->RegisterFound(FragmentId);
				bFound = true;
				UE_LOG(LogLongNoon, Log, TEXT("[Lore] Fragment read: %s."), *FragmentId.ToString());
				SetActorHiddenInGame(true);
				SetActorEnableCollision(false);
			}
		}
	}
}

FText ALongNoonLoreFragment::GetInteractPrompt_Implementation() const
{
	return NSLOCTEXT("LongNoon", "ReadPrompt", "Read");
}
