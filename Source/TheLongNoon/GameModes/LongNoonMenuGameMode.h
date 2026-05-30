#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LongNoonMenuGameMode.generated.h"

class ULongNoonMainMenuWidget;

/**
 * GameMode for the title-screen level. On BeginPlay it creates the C++ main-menu
 * widget, adds it to the viewport, and hands it keyboard focus (UI-only input).
 * Set as the level's GameMode override on L_MainMenu. See docs/tech/build-and-run.md.
 */
UCLASS()
class THELONGNOON_API ALongNoonMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALongNoonMenuGameMode();

protected:
	virtual void BeginPlay() override;

	/** Menu widget class (defaults to the C++ ULongNoonMainMenuWidget). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Menu")
	TSubclassOf<ULongNoonMainMenuWidget> MenuWidgetClass;
};
