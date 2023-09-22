// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Widgets/Title/UserWidget_MainTitle.h"
#include "projectJG_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTJG_API UprojectJG_GameInstance : public UGameInstance
{
	GENERATED_BODY()
private:
	//LocalData
	float BackgroundVolume = 1.0f;
	float EffectVolume = 1.0f;

	FGameSettingData StaticData;
	//uint8 MapData;
	//uint8 CharacterData;
	//uint8 Difficulty;
public:
	void SetBackgroundVolume(float value);
	void SetEffectVolume(float value);

	void SetGameData(FGameSettingData Data);

	FGameSettingData GetData() { return StaticData; }
	float GetBackgroundVolume() {return BackgroundVolume;}
	float GetEffectVolume() { return EffectVolume; }


};
