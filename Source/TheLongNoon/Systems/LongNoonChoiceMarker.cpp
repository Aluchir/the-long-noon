#include "Systems/LongNoonChoiceMarker.h"
#include "Systems/LongNoonEndings.h"
#include "Core/LongNoonGameInstance.h"
#include "Core/LongNoonLog.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"

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
	const UEnum* EndingEnum = StaticEnum<ELongNoonEnding>();
	const FString Name = EndingEnum ? EndingEnum->GetDisplayNameTextByValue((int64)Resolved).ToString() : TEXT("?");
	UE_LOG(LogLongNoon, Log, TEXT("[Ending] Marker chosen -> %s (rememberer=%s)"),
		*Name, bRemembererFound ? TEXT("yes") : TEXT("no"));
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
