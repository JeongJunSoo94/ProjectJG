#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/CharacterSkillWidget.h"
#include "Character/Components/CActionComponent.h"
#include "PlayerInGameWidget.generated.h"

UCLASS()
class PROJECTJG_API UPlayerInGameWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	//UPlayerInGameWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeOnInitialized() override;


	UFUNCTION(BlueprintImplementableEvent)
		void SetInit(UBaseHUDComponent* HUD);

	UFUNCTION(BlueprintCallable)
		void SetBaseWidget(UBaseHUDComponent* HUD,UHealthWidget* HealthWidget, UCharacterSkillWidget* abilityLeft, UCharacterSkillWidget* abilityMid, UCharacterSkillWidget* abilityRight);
	UFUNCTION(BlueprintCallable)
		void Update(float curTime, float maxTime);

	UFUNCTION(BlueprintCallable)
		ACBaseCharacter* GetCharacter() {return player;}
private:
	ACBaseCharacter* player;
};
