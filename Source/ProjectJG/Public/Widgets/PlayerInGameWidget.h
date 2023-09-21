#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/CharacterSkillWidget.h"
#include "Widgets/HealthWidget.h"
#include "Character/Components/CActionComponent.h"
#include "PlayerInGameWidget.generated.h"

UCLASS()
class PROJECTJG_API UPlayerInGameWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintCallable)
		void Update(float curTime, float maxTime);

	UFUNCTION(BlueprintCallable)
		ACBaseCharacter* GetCharacter() {return player;}

	UHealthWidget* GetWB_Health() { return WB_Health;}
	UCharacterSkillWidget* GetWB_CharacterSkill_Left() {return WB_CharacterSkill_Left;}
	UCharacterSkillWidget* GetWB_CharacterSkill_Mid() { return WB_CharacterSkill_Mid; }
	UCharacterSkillWidget* GetWB_CharacterSkill_Right() { return WB_CharacterSkill_Right; }
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UHealthWidget* WB_Health;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UCharacterSkillWidget* WB_CharacterSkill_Left;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UCharacterSkillWidget* WB_CharacterSkill_Mid;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UCharacterSkillWidget* WB_CharacterSkill_Right;
private:
	ACBaseCharacter* player;
};
