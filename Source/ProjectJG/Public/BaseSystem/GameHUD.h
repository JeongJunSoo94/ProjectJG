#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Character/Components/CActionComponent.h"
#include "GameHUD.generated.h"

UCLASS()
class PROJECTJG_API AGameHUD : public AHUD
{
	GENERATED_BODY()
public:
	AGameHUD();
	virtual void BeginPlay()override;
private:
	TSubclassOf<class UUserWidget>BP_UIWidgetClass;
	UUserWidget* UIWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<class UUserWidget_CrossHair> CrossHairClass;

	class UUserWidget_CrossHair* CrossHair;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<class UPlayerInGameWidget> PlayerInGameClass;

	class UPlayerInGameWidget* PlayerInGameUI;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<class UInGameMenuUserWidget> InGameMenuClass;

	class UInGameMenuUserWidget* InGameMenuUI;


	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<class USoundControlWidget> SoundControlClass;

	class USoundControlWidget* SoundControlUI;

public:
	virtual void SetHUDPlayerControllerSkillBind(UActorComponent* Weapon = nullptr, UCActionComponent* Left = nullptr, UCActionComponent* Mid = nullptr, UCActionComponent* Right = nullptr);
	virtual void HealthBarUpdate(float curHealth, float maxHealth);
	void IsMenuBarActive(bool isActive);
	void IsSoundActive(bool isActive);
public:
	bool bPause = false;
};
