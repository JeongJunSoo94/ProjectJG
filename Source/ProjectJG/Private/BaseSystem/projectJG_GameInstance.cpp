// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseSystem/projectJG_GameInstance.h"
#include "Global.h"



void UprojectJG_GameInstance::SetBackgroundVolume(float value)
{
	BackgroundVolume = value;
}

void UprojectJG_GameInstance::SetEffectVolume(float value)
{
	EffectVolume = value;
}

void UprojectJG_GameInstance::SetGameData(FGameSettingData Data)
{
	StaticData.StageNumber = Data.StageNumber;
	StaticData.SelectCharacter = Data.SelectCharacter;
	StaticData.Difficulty = Data.Difficulty;
}
