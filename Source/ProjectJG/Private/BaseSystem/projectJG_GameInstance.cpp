#include "BaseSystem/projectJG_GameInstance.h"
#include "Global.h"

UprojectJG_GameInstance::UprojectJG_GameInstance()
{
}

void UprojectJG_GameInstance::OnStart()
{
	Super::OnStart();

}

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

