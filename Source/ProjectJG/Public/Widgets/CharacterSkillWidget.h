#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSkillWidget.generated.h"

UCLASS()
class PROJECTJG_API UCharacterSkillWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	//UCharacterSkillWidget(const FObjectInitializer &ObjectInitializer);
	virtual void NativeOnInitialized();

	UFUNCTION(BlueprintImplementableEvent)
		void SetInit(UTexture2D* image=nullptr);
	UFUNCTION(BlueprintImplementableEvent)
		void Update(float curTime, float maxTime);
};