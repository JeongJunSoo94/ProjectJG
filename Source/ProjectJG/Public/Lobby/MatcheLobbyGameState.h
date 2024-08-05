// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MatcheLobbyGameState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTJG_API AMatcheLobbyGameState : public AGameStateBase
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void UpdatePlayerControllers();
	void UpdateTest();
	
	UPROPERTY(ReplicatedUsing = OnRep_PlayerInfos, VisibleAnywhere, Category = "Players")
		TArray<int32> PlayerInfos;
	UFUNCTION()
		void OnRep_PlayerInfos();

	UPROPERTY(ReplicatedUsing = OnRep_PlayerReadys, VisibleAnywhere, Category = "Players")
		TArray<bool> PlayerReadys;
	UFUNCTION()
		void OnRep_PlayerReadys();

	class UMatcheLobbyUserWidget* MatcheMenu;

	/** Add PlayerState to the PlayerArray */
	virtual void AddPlayerState(APlayerState* PlayerState) override;

	/** Remove PlayerState from the PlayerArray. */
	virtual void RemovePlayerState(APlayerState* PlayerState) override;

private:
	FTimerHandle PlayerCheckDataTimerHandle;
};
