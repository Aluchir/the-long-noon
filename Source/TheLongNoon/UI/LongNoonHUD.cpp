#include "UI/LongNoonHUD.h"
#include "UI/LongNoonHUDWidget.h"
#include "Blueprint/UserWidget.h"

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
