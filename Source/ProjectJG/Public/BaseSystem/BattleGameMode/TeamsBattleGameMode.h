// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSystem/BattleGameMode.h"
#include "TeamsBattleGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTJG_API ATeamsBattleGameMode : public ABattleGameMode
{
	GENERATED_BODY()
public:
	ATeamsBattleGameMode();
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual float CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage) override;
	virtual void PlayerEliminated(class ABaseCharacter* ElimmedCharacter, class AInGamePlayerController* VictimController, AInGamePlayerController* AttackerController) override;
protected:
	virtual void HandleMatchHasStarted() override;
};
