// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "InGameStateBase.generated.h"

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

private:
	int curKillNumber = 0;
	uint8 curKeyValue = 0;
public:
	UPROPERTY(EditAnywhere)
		int GoalKillNumber = -1;
	UPROPERTY(EditAnywhere)
		uint8 GoalKeyValue = 0;

	void StoreKey(uint8 KeyNumber);
	void KillEnemy(int killScore);

	bool IsGoal();
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
	void EndSection();

	void TakeKey(uint8 KeyNumber);
	FClearCondition* GetClearCondition();
	void SetMediator(ASectionMediator* SectionMediator);
};
