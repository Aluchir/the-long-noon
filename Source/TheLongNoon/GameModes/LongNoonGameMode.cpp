#include "GameModes/LongNoonGameMode.h"
#include "Player/LongNoonCharacter.h"
#include "Player/LongNoonPlayerController.h"

ALongNoonGameMode::ALongNoonGameMode()
{
	DefaultPawnClass = ALongNoonCharacter::StaticClass();
	PlayerControllerClass = ALongNoonPlayerController::StaticClass();
}
