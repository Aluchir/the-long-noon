#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LongNoonPlayerController.generated.h"

/**
 * Player controller for The Long Noon. Thin for now; owns player-scoped concerns
 * (UI/HUD wiring, save triggers) as they are implemented. Input mapping context is
 * applied by the character on possession.
 */
UCLASS()
class THELONGNOON_API ALongNoonPlayerController : public APlayerController
{
	GENERATED_BODY()
};
