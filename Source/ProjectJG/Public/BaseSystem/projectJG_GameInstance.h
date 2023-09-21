// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
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

	uint8 MapData;
	uint8 CharacterData;
	uint8 Difficulty;
public:
	void SetBackgroundVolume(float value);
	void SetEffectVolume(float value);

	float GetBackgroundVolume() {return BackgroundVolume;}
	float GetEffectVolume() { return EffectVolume; }
};
