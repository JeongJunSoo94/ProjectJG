// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "InGameStateBase.generated.h"

DECLARE_DELEGATE(FOnUpdateQuest);
/**
 * 
 */
UENUM(BlueprintType)
enum class EGameState :uint8
{
	GameStart,
	GamePlay,
	GamePause,
	GameOver,
};

USTRUCT(BlueprintType)
struct FClearCondition
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int curKillNumber = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		uint8 curKeyValue = 0;
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int GoalKillNumber = -1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		uint8 GoalKeyValue = 0;

	void StoreKey(uint8 KeyNumber);
	void KillEnemy(int killScore);

	bool IsGoal();
	
	int GetCurKillNumber() { return curKillNumber; }
	uint8 GetCurKeyValue() { return curKeyValue; }

};

UCLASS()
class PROJECTJG_API AInGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
		EGameState curState = EGameState::GameStart;
	UFUNCTION(BlueprintCallable)
		void ChangeGameState(EGameState NextState);
	UPROPERTY(EditAnywhere)
		class ASectionMediator* curSection;
	UPROPERTY(EditAnywhere)
		TMap<ASectionMediator*,FClearCondition> Map_SectionClearConditions;


public:
	UPROPERTY(Transient, BlueprintReadOnly, Category = GameState)
		TArray<TObjectPtr<class AInGamePlayerState>> InGamePlayerArray;

	virtual void PostInitializeComponents() override;

	void StartSection(class ASectionMediator* SectionData);
	bool EndSection();

	void TakeKey(uint8 KeyNumber);
	FClearCondition* GetClearCondition();
	void UpdateKillScore();

	FOnUpdateQuest OnEventUpdateSection;
	FOnUpdateQuest OnEventUpdateData;

};
