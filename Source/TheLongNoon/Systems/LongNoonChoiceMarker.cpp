#include "Systems/LongNoonChoiceMarker.h"
#include "Systems/LongNoonEndings.h"
#include "Core/LongNoonGameInstance.h"
#include "Core/LongNoonLog.h"
#include "UI/LongNoonHUD.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "GameFramework/PlayerController.h"

ALongNoonChoiceMarker::ALongNoonChoiceMarker()
{
	PrimaryActorTick.bCanEverTick = false;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MarkerMesh"));
	SetRootComponent(Mesh);
}

void ALongNoonChoiceMarker::OnInteract_Implementation(AActor* /*Interactor*/)
{
	bool bRemembererFound = false;
	if (const UWorld* World = GetWorld())
	{
		if (const ULongNoonGameInstance* GI = Cast<ULongNoonGameInstance>(World->GetGameInstance()))
		{
			bRemembererFound = GI->bRemembererFound;
		}
	}

	Resolved = ULongNoonEndingLibrary::ResolveEnding(Choice, bRemembererFound);

	const FText Title = ULongNoonEndingLibrary::GetEndingTitle(Resolved);
	const FText Body  = ULongNoonEndingLibrary::GetEndingText(Resolved);

	UE_LOG(LogLongNoon, Log, TEXT("[Ending] Marker chosen -> %s (rememberer=%s)"),
		*Title.ToString(), bRemembererFound ? TEXT("yes") : TEXT("no"));
	UE_LOG(LogLongNoon, Log, TEXT("[Ending] %s"), *Body.ToString());

	// Surface the ending beat on the HUD if one is present.
	if (const UWorld* World = GetWorld())
	{
		if (const APlayerController* PC = World->GetFirstPlayerController())
		{
			if (ALongNoonHUD* HUD = Cast<ALongNoonHUD>(PC->GetHUD()))
			{
				HUD->ShowToast(FText::Format(
					NSLOCTEXT("LongNoon", "EndingToast", "{0}\n\n{1}"), Title, Body));
			}
		}
	}
}

FText ALongNoonChoiceMarker::GetInteractPrompt_Implementation() const
{
	switch (Choice)
	{
	case ELongNoonChoice::GiveItBack:  return NSLOCTEXT("LongNoon", "ChGive",  "Give the ending back");
	case ELongNoonChoice::KeepTheNoon: return NSLOCTEXT("LongNoon", "ChKeep",  "Keep the Long Noon");
	case ELongNoonChoice::TheThirdWay: return NSLOCTEXT("LongNoon", "ChThird", "Return their memory");
	default:                           return NSLOCTEXT("LongNoon", "ChChoose", "Choose");
	}
}
