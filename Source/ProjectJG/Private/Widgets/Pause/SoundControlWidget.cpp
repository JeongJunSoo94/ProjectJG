// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Pause/SoundControlWidget.h"
#include "Global.h"
#include "Components/Slider.h"
#include "Sound/SoundClass.h"

void USoundControlWidget::Change_BackgroundSound(float value)
{
	if (!GameInstance)
	{
		CachingGameInstance();
	}

	GameInstance->SetBackgroundVolume(value);
	Clog::Log(GameInstance->GetBackgroundVolume());

}

void USoundControlWidget::Change_EffectSound(float value)
{
	if (!GameInstance)
	{
		CachingGameInstance();
	}

	GameInstance->SetEffectVolume(value);
	Clog::Log(GameInstance->GetEffectVolume());
}

void USoundControlWidget::CachingGameInstance()
{
	GameInstance = Cast<UprojectJG_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}


void USoundControlWidget::Setting_BackgroundSound()
{
	if (!GameInstance)
	{
		CachingGameInstance();
	}
	BackgroundSound_Slider->SetValue(GameInstance->GetBackgroundVolume());


}

void USoundControlWidget::Setting_EffectSound()
{
	if (!GameInstance)
	{
		CachingGameInstance();
	}
	EffectSound_Slider->SetValue(GameInstance->GetEffectVolume());
}

