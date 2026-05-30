#include "Systems/LongNoonLoreFragment.h"
#include "Systems/LongNoonCodexSubsystem.h"
#include "Components/StaticMeshComponent.h"
#include "Core/LongNoonGameInstance.h"
#include "Core/LongNoonLog.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "UObject/ConstructorHelpers.h"

ALongNoonLoreFragment::ALongNoonLoreFragment()
{
	PrimaryActorTick.bCanEverTick = false;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FragmentMesh"));
	SetRootComponent(Mesh);

	// Default read cue: the CC0 "bookOpen" SFX (Kenney RPG Audio). Editable per instance.
	static ConstructorHelpers::FObjectFinder<USoundBase> ReadCue(TEXT("/Game/ThirdParty/Audio/SFX/bookOpen.bookOpen"));
	if (ReadCue.Succeeded())
	{
		ReadSound = ReadCue.Object;
	}
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

				if (ReadSound)
				{
					UGameplayStatics::PlaySound2D(this, ReadSound);
				}

				if (bGrantsRememberer)
				{
					if (ULongNoonGameInstance* LNGI = Cast<ULongNoonGameInstance>(GI))
					{
						LNGI->SetRemembererFound();
					}
				}

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
