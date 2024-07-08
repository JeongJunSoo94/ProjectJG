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
private:
	UPROPERTY(Replicated, VisibleAnywhere, Category = "Players")
		int32 NumPublicConnections = 0;
protected:
	virtual void BeginPlay() override;

	void UpdatePlayerControllers();


public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable)
		void SetNumPublicConnections();

	TArray<APlayerController*> PlayerControllers;
	class UMatcheLobbyUserWidget* MatcheMenu;
};
