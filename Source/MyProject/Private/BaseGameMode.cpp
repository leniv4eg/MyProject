// My first Unreal Engine 5 Game project


#include "BaseGameMode.h"
#include "Player/BaseGameCharacter.h"
#include "Player/BaseGamePlayerController.h"

ABaseGameMode::ABaseGameMode()
{
	DefaultPawnClass = ABaseGameCharacter::StaticClass();
	PlayerControllerClass = ABaseGamePlayerController::StaticClass();
}
