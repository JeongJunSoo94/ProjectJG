#include "BaseSystem/StartGameHUD.h"
#include "Global.h"
#include "Blueprint/UserWidget.h"

AStartGameHUD::AStartGameHUD()
{
	CHelpers::GetClass<UUserWidget>(&BP_UIWidgetClass, "WidgetBlueprint'/Game/Developers/GohyeongJu/Characters/Level/Wigets/WB_MainTitle.WB_MainTitle_C'");
}

void AStartGameHUD::BeginPlay()
{
	Super::BeginPlay();
	if (BP_UIWidgetClass)
	{
		UIWidget = CreateWidget<UUserWidget>(GetWorld(), BP_UIWidgetClass);
		if (UIWidget)
		{
			UIWidget->AddToViewport();
		}
	}
}