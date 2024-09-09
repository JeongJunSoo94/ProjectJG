// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BattleGameMode.generated.h"

namespace MatchState
{
	extern PROJECTJG_API const FName Cooldown;
}

UCLASS()
class PROJECTJG_API ABattleGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	ABattleGameMode();
	virtual void Tick(float DeltaTime) override;
	virtual void PlayerEliminated(class ABaseCharacter* ElimmedCharacter, class AInGamePlayerController* VictimController, class AInGamePlayerController* AttackerController);
	virtual void PlayerEliminated(class ABaseCharacter* ElimmedCharacter, class AInGamePlayerController* VictimController, class ABaseAIController* AttackerController);
	virtual void EnemyEliminated(class ABaseEnemyCharacter* ElimmedCharacter, class ABaseAIController* VictimController, class AInGamePlayerController* AttackerController);
	virtual void RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController);
	//virtual void RestartPlayer(AController* NewPlayer);
	
	
	void PlayerLeftGame(class AInGamePlayerState* PlayerLeaving);
	virtual float CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage);
	UPROPERTY(EditDefaultsOnly)
		float WarmupTime = 5.f;

	UPROPERTY(EditDefaultsOnly)
		float MatchTime = 600.f;

	UPROPERTY(EditDefaultsOnly)
		float CooldownTime = 5.f;

	float LevelStartingTime = 0.f;

	bool bTeamsMatch = false;
protected:
	virtual void BeginPlay() override;
	virtual void OnMatchStateSet() override;

private:
	float CountdownTime = 0.f;
public:
	FORCEINLINE float GetCountdownTime() const { return CountdownTime; }
};
