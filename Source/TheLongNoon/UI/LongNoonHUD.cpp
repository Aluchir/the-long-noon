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
