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

	void UpdateMatcheItems();
	
	UPROPERTY(ReplicatedUsing = OnRep_PlayersInfo, VisibleAnywhere, Category = "Players")
		TArray<int32> PlayersInfo;
	UFUNCTION()
		void OnRep_PlayersInfo();

	UPROPERTY(ReplicatedUsing = OnRep_PlayerReadys, VisibleAnywhere, Category = "Players")
		TArray<bool> PlayerReadys;
	UFUNCTION()
		void OnRep_PlayerReadys();

	class UMatcheLobbyUserWidget* MatcheMenu;

	virtual void AddPlayerState(APlayerState* PlayerState) override;

	virtual void RemovePlayerState(APlayerState* PlayerState) override;

	APlayerState* GetFindPlayerState(int32 SlotIdx);

private:
	FTimerHandle PlayerCheckDataTimerHandle;
};
