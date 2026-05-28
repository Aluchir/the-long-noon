#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LongNoonGameMode.generated.h"

/**
 * Default game mode. Sets the Newcomer pawn and the player controller. Per-region
 * variants (or data on the RegionDef) can override defaults as content grows.
 * See docs/tech/coding-standards-and-architecture.md.
 */
UCLASS()
class THELONGNOON_API ALongNoonGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALongNoonGameMode();
};
