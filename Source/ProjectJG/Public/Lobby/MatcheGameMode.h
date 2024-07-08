// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MatcheGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTJG_API AMatcheGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	void TravelToMap(const FString& MapName);

	int32 GetSlotIdx(APlayerController* Controller);
	int32 GetSessionPlayerNum() const;
private:
	
	TArray<APlayerController*> PlayerControllers;

	class AMatchePlayerController* HostController;

	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;
};
