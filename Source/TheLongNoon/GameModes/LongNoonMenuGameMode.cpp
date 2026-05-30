#include "GameModes/LongNoonMenuGameMode.h"
#include "UI/LongNoonMainMenuWidget.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

ALongNoonMenuGameMode::ALongNoonMenuGameMode()
{
	MenuWidgetClass = ULongNoonMainMenuWidget::StaticClass();
}

void ALongNoonMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!PC || !MenuWidgetClass)
	{
		return;
	}

	ULongNoonMainMenuWidget* Menu = CreateWidget<ULongNoonMainMenuWidget>(PC, MenuWidgetClass);
	if (!Menu)
	{
		return;
	}
	Menu->AddToViewport(50);

	FInputModeUIOnly Mode;
	Mode.SetWidgetToFocus(Menu->TakeWidget());
	Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PC->SetInputMode(Mode);
	PC->bShowMouseCursor = true;
}
