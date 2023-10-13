#include "Widgets/Pause/InGameMenuUserWidget.h"
#include "Global.h"
#include "BaseSystem/GameHUD.h"


UInGameMenuUserWidget::UInGameMenuUserWidget(const FObjectInitializer& ObjectInitializer)	: Super(ObjectInitializer)
{
}

void UInGameMenuUserWidget::Resume()
{
	AGameHUD* GameHUD = UGameplayStatics::GetPlayerController(this, 0)->GetHUD<AGameHUD>();
	CheckNull(GameHUD);
	GameHUD->IsMenuBarActive(false);
}

void UInGameMenuUserWidget::Lobby() 
{
	UGameplayStatics::OpenLevel(GetWorld(), "Title");
}
void UInGameMenuUserWidget::Option() 
{
	AGameHUD* GameHUD = UGameplayStatics::GetPlayerController(this, 0)->GetHUD<AGameHUD>();
	CheckNull(GameHUD);
	GameHUD->IsSoundActive(true);
}
void UInGameMenuUserWidget::Exit() 
{
}