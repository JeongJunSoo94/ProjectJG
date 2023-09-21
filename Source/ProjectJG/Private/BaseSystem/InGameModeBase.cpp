#include "BaseSystem/InGameModeBase.h"
#include "Global.h"
#include "BaseSystem/GameHUD.h"
//#include "BaseSystem/projectJG_GameInstance.h"
#include "GameFramework/Pawn.h"

AInGameModeBase::AInGameModeBase()
{
	HUDClass = AGameHUD::StaticClass();
	TSubclassOf<APawn> PawnClass;
	CHelpers::GetClass<APawn>(&PawnClass, "Blueprint'/Game/Developers/USER/Character/BP_LtBelica.BP_LtBelica_C'");
	DefaultPawnClass = PawnClass;

	//PlayerControllerClass = AController_StartMenu::StaticClass();
}

void AInGameModeBase::BeginPlay()
{
	//= Cast<UprojectJG_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//DefaultPawnClass = ;
	Super::BeginPlay();

}