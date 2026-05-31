#include "Systems/LongNoonChoiceMarker.h"
#include "Systems/LongNoonEndings.h"
#include "Core/LongNoonGameInstance.h"
#include "Core/LongNoonLog.h"
#include "UI/LongNoonEndingWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

ALongNoonChoiceMarker::ALongNoonChoiceMarker()
{
	PrimaryActorTick.bCanEverTick = false;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MarkerMesh"));
	SetRootComponent(Mesh);
}

void ALongNoonChoiceMarker::OnInteract_Implementation(AActor* Interactor)
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

	// Resolve the player controller (the interactor's, falling back to the first PC).
	APlayerController* PC = nullptr;
	if (const APawn* Pawn = Cast<APawn>(Interactor))
	{
		PC = Cast<APlayerController>(Pawn->GetController());
	}
	if (!PC && GetWorld())
	{
		PC = GetWorld()->GetFirstPlayerController();
	}
	if (!PC)
	{
		return;
	}

	// Show the full-screen ending sequence and hand it keyboard focus.
	if (ULongNoonEndingWidget* End = CreateWidget<ULongNoonEndingWidget>(PC, ULongNoonEndingWidget::StaticClass()))
	{
		End->SetEnding(Title, Body);
		End->AddToViewport(200);
		FInputModeUIOnly Mode;
		Mode.SetWidgetToFocus(End->TakeWidget());
		Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PC->SetInputMode(Mode);
		PC->bShowMouseCursor = true;
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
