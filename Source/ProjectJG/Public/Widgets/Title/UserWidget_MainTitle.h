// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidget_MainTitle.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EDifficulty : uint8
{
	EEasy = 0		UMETA(DisplayName = "Easy"),
	ENormal			UMETA(DisplayName = "Normal"),
	EHard			UMETA(DisplayName = "Hard"),
};

UENUM(BlueprintType)
enum class EStageNumber : uint8
{
	EStage0 = 0		UMETA(DisplayName = "Stage0"),
	EStage1 		UMETA(DisplayName = "Stage1"),
	EStage2 		UMETA(DisplayName = "Stage2"),

};

UENUM(BlueprintType)
enum class ECharacterSelect : uint8
{
	None = 0			UMETA(DisplayName = "None"),
	EBelica 			UMETA(DisplayName = "Belica"),
	EMurdock			UMETA(DisplayName = "Murdock"),
	EUnkonwn_0			UMETA(DisplayName = "Unkonwn_0"),
	EUnkonwn_1			UMETA(DisplayName = "Unkonwn_1"),
};

USTRUCT(BlueprintType)
struct FGameSettingData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		EStageNumber StageNumber;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		EDifficulty	Difficulty;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		ECharacterSelect SelectCharacter;
};

namespace StageData
{
	const FName Stage0 = TEXT("Stage0");
	const FName Stage1 = TEXT("Stage1");
	const FName Stage2 = TEXT("Stage2");
}

namespace Difficulty
{
	const FName Easy	= TEXT("Easy");
	const FName Normal	= TEXT("Normal");
	const FName Hard	= TEXT("Hard");
}

UCLASS()
class PROJECTJG_API UUserWidget_MainTitle : public UUserWidget
{
	GENERATED_BODY()
public:
	UUserWidget_MainTitle(const FObjectInitializer& ObjectInitializer);
protected:
	//UPROPERTY(BlueprintReadOnly, EditAnywhere)
		//class UComboBoxKey* StageComboBox_Key;

	TArray<FName> StageFNameData; // 외부 데이터에 있다고 생각

public:
	UFUNCTION(BlueprintCallable)
		void SelectStage(FName StageNumber);
	UFUNCTION(BlueprintCallable)
		void SelectDifficulty(FName Difficulty);
	UFUNCTION(BlueprintCallable)
		void SelectCharacter(ECharacterSelect SelectCharacterEnum);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FGameSettingData GameData;

	UFUNCTION(BlueprintCallable)
		void PrintStageNumber();
	UFUNCTION(BlueprintCallable)
		void PrintDifficulty();
	UFUNCTION(BlueprintCallable)
		void PrintCharacterEnum();

	UFUNCTION(BlueprintCallable)
		void OnStartButton(FName StageName);

};
