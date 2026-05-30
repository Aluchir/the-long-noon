#include "UI/LongNoonHUD.h"
#include "UI/LongNoonHUDWidget.h"
#include "Blueprint/UserWidget.h"

ALongNoonHUD::ALongNoonHUD()
{
	// Default to the functional C++ HUD widget; a WBP child can override this later.
	HUDWidgetClass = ULongNoonHUDWidget::StaticClass();
}

void ALongNoonHUD::BeginPlay()
{
	Super::BeginPlay();

	if (HUDWidgetClass && GetOwningPlayerController())
	{
		HUDWidget = CreateWidget<ULongNoonHUDWidget>(GetOwningPlayerController(), HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
		}
	}
}

void ALongNoonHUD::UpdateTend(float Stamina, float Focus, float Comfort)
{
	if (HUDWidget)
	{
		HUDWidget->UpdateTend(Stamina, Focus, Comfort);
	}
}

void ALongNoonHUD::SetInteractPrompt(const FText& Prompt)
{
	if (HUDWidget)
	{
		HUDWidget->SetInteractPrompt(Prompt);
	}
}

void ALongNoonHUD::NotifyLoreFound(FName FragmentId)
{
	if (HUDWidget)
	{
		HUDWidget->OnLoreFound(FragmentId);
	}
}

void ALongNoonHUD::ShowToast(const FText& Message)
{
	if (HUDWidget)
	{
		HUDWidget->ShowToast(Message);
	}
}
