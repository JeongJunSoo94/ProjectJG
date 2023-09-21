#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "StartGameHUD.generated.h"

UCLASS()
class PROJECTJG_API AStartGameHUD : public AHUD
{
	GENERATED_BODY()
public:
	AStartGameHUD();
	void BeginPlay()override;

private:
	TSubclassOf<class UUserWidget>BP_UIWidgetClass;
	UUserWidget* UIWidget;
};
