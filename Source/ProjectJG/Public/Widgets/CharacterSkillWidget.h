// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSkillWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTJG_API UCharacterSkillWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintImplementableEvent)
		void SetInit(UTexture2D* image);
	UFUNCTION(BlueprintImplementableEvent)
		void Update(float curTime, float maxTime);
};
