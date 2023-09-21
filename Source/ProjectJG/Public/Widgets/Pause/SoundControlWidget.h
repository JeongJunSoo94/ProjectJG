// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseSystem/projectJG_GameInstance.h"
#include "SoundControlWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTJG_API USoundControlWidget : public UUserWidget
{
	GENERATED_BODY()

protected:


	UFUNCTION(BlueprintCallable)
		void Change_BackgroundSound(float value);
	UFUNCTION(BlueprintCallable)
		void Change_EffectSound(float value);
	UFUNCTION(BlueprintCallable)
		void CachingGameInstance();

	UFUNCTION(BlueprintCallable)
		void Setting_BackgroundSound();
	UFUNCTION(BlueprintCallable)
		void Setting_EffectSound();

	//void CachingEffectSoundClass();
	//void CachingBackgroundSoundClass();
public:
	UPROPERTY(EditAnywhere, Category = "BackgroundVolume", meta = (BindWidget))
		class USlider* BackgroundSound_Slider;
	UPROPERTY(EditAnywhere, Category = "EffectVolume", meta = (BindWidget))
		class USlider* EffectSound_Slider;
private:
	UPROPERTY(VisibleDefaultsOnly)
		class UprojectJG_GameInstance* GameInstance = nullptr;

	//UPROPERTY(VisibleDefaultsOnly)
	//	class USoundClass* EffectSoundClass;
	//UPROPERTY(VisibleDefaultsOnly)
	//	class USoundClass* BackgroundSoundClass;
};
