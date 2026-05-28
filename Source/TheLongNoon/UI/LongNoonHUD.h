#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LongNoonHUD.generated.h"

class ULongNoonHUDWidget;

/**
 * Creates and owns the in-game HUD widget. Assign HUDWidgetClass to a WBP that
 * derives from ULongNoonHUDWidget. See the UI module and docs/art/style-guide.md.
 */
UCLASS()
class THELONGNOON_API ALongNoonHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
	TSubclassOf<ULongNoonHUDWidget> HUDWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "HUD")
	TObjectPtr<ULongNoonHUDWidget> HUDWidget;
};
