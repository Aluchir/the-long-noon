#include "GameModes/LongNoonGameMode.h"
#include "Player/LongNoonCharacter.h"
#include "Player/LongNoonPlayerController.h"
#include "UI/LongNoonHUD.h"

ALongNoonGameMode::ALongNoonGameMode()
{
	DefaultPawnClass = ALongNoonCharacter::StaticClass();
	PlayerControllerClass = ALongNoonPlayerController::StaticClass();
	HUDClass = ALongNoonHUD::StaticClass();
}
