#include "Systems/LongNoonCraftingStation.h"
#include "Systems/LongNoonCraftingComponent.h"
#include "UI/LongNoonCraftingMenuWidget.h"
#include "Components/StaticMeshComponent.h"
#include "Blueprint/UserWidget.h"
#include "Core/LongNoonLog.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"

ALongNoonCraftingStation::ALongNoonCraftingStation()
{
	PrimaryActorTick.bCanEverTick = false;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StationMesh"));
	SetRootComponent(Mesh);
}

void ALongNoonCraftingStation::OnInteract_Implementation(AActor* Interactor)
{
	if (!Interactor)
	{
		return;
	}

	ULongNoonCraftingComponent* Crafting = Interactor->FindComponentByClass<ULongNoonCraftingComponent>();
	APlayerController* PC = nullptr;
	if (const APawn* Pawn = Cast<APawn>(Interactor))
	{
		PC = Cast<APlayerController>(Pawn->GetController());
	}
	if (!Crafting || !PC)
	{
		return;
	}

	ULongNoonCraftingMenuWidget* Menu = CreateWidget<ULongNoonCraftingMenuWidget>(PC, ULongNoonCraftingMenuWidget::StaticClass());
	if (!Menu)
	{
		return;
	}
	Menu->Init(Crafting, StationId);
	Menu->AddToViewport(90);

	FInputModeUIOnly Mode;
	Mode.SetWidgetToFocus(Menu->TakeWidget());
	Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PC->SetInputMode(Mode);
	PC->bShowMouseCursor = true;

	UE_LOG(LogLongNoon, Log, TEXT("[Craft] Opened station %s."), *StationId.ToString());
}

FText ALongNoonCraftingStation::GetInteractPrompt_Implementation() const
{
	return NSLOCTEXT("LongNoon", "CraftStationPrompt", "Craft");
}
