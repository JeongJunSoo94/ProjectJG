#include "BaseSystem/StartGameModeBase.h"
#include "Global.h"
#include "BaseSystem/StartGameHUD.h"

AStartGameModeBase::AStartGameModeBase()
{
	HUDClass = AStartGameHUD::StaticClass();
	DefaultPawnClass = nullptr;
	//PlayerControllerClass = AController_StartMenu::StaticClass();

}