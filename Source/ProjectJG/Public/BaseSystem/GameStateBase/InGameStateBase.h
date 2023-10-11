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

public:
	UPROPERTY(Transient, BlueprintReadOnly, Category = GameState)
		TArray<TObjectPtr<class AInGamePlayerState>> InGamePlayerArray;

	virtual void PostInitializeComponents() override;

	void StartSection(class ASectionMediator* SectionData);
	void EndSection();
};
