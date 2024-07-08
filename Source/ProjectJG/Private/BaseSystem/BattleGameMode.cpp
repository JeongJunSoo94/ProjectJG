#include "BaseSystem/BattleGameMode.h"
#include "Global.h"
#include "GameFramework/PlayerStart.h"
#include "BaseSystem/InGamePlayerController.h"
#include "BaseSystem/PlayerState/InGamePlayerState.h"
#include "BaseSystem/GameStateBase/InGameStateBase.h"
#include "BaseSystem/BattleGameState.h"
#include "Character/BaseCharacter.h"
#include "Character/Enemies/BaseEnemyCharacter.h"
#include "Character/Enemies/AIController/BaseAIController.h"

namespace MatchState
{
	const FName Cooldown = FName("Cooldown");
}

ABattleGameMode::ABattleGameMode()
{
	bDelayedStart = true;
}

void ABattleGameMode::BeginPlay()
{
	Super::BeginPlay();

	LevelStartingTime = GetWorld()->GetTimeSeconds();
}

void ABattleGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MatchState == MatchState::WaitingToStart)
	{
		CountdownTime = WarmupTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f)
		{
			StartMatch();
		}
	}
	else if (MatchState == MatchState::InProgress)
	{
		CountdownTime = WarmupTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f)
		{
			SetMatchState(MatchState::Cooldown);
		}
	}
	else if (MatchState == MatchState::Cooldown)
	{
		CountdownTime = CooldownTime + WarmupTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f)
		{
			RestartGame();
		}
	}
}

void ABattleGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		AInGamePlayerController* PlayerController = Cast<AInGamePlayerController>(*It);
		if (PlayerController)
		{
			PlayerController->OnMatchStateSet(MatchState, bTeamsMatch);
		}
	}
}

float ABattleGameMode::CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage)
{
	return BaseDamage;
}

void ABattleGameMode::PlayerEliminated(class ABaseCharacter* ElimmedCharacter, class AInGamePlayerController* VictimController, class AInGamePlayerController* AttackerController)
{
	if (AttackerController == nullptr || AttackerController->PlayerState == nullptr) return;
	if (VictimController == nullptr || VictimController->PlayerState == nullptr) return;
	AInGamePlayerState* AttackerPlayerState = AttackerController ? Cast<AInGamePlayerState>(AttackerController->PlayerState) : nullptr;
	AInGamePlayerState* VictimPlayerState = VictimController ? Cast<AInGamePlayerState>(VictimController->PlayerState) : nullptr;

	ABattleGameState* BattleGameState = GetGameState<ABattleGameState>();

	if (AttackerPlayerState && AttackerPlayerState != VictimPlayerState && BattleGameState)
	{
		TArray<AInGamePlayerState*> PlayersCurrentlyInTheLead;
		for (auto LeadPlayer : BattleGameState->TopScoringPlayers)
		{
			PlayersCurrentlyInTheLead.Add(LeadPlayer);
		}

		AttackerPlayerState->AddToScore(1.f);
		BattleGameState->UpdateTopScore(AttackerPlayerState);
		if (BattleGameState->TopScoringPlayers.Contains(AttackerPlayerState))
		{
			ABaseCharacter* Leader = Cast<ABaseCharacter>(AttackerPlayerState->GetPawn());
			if (Leader)
			{
				Leader->MulticastGainedTheLead();
			}
		}

		for (int32 i = 0; i < PlayersCurrentlyInTheLead.Num(); i++)
		{
			if (!BattleGameState->TopScoringPlayers.Contains(PlayersCurrentlyInTheLead[i]))
			{
				ABaseCharacter* Loser = Cast<ABaseCharacter>(PlayersCurrentlyInTheLead[i]->GetPawn());
				if (Loser)
				{
					Loser->MulticastLostTheLead();
				}
			}
		}
	}
	if (VictimPlayerState)
	{
		VictimPlayerState->AddToDefeats(1);
	}

	if (ElimmedCharacter)
	{
		ElimmedCharacter->Elim(false);
	}

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		AInGamePlayerController* BlasterPlayer = Cast<AInGamePlayerController>(*It);
		if (BlasterPlayer && AttackerPlayerState && VictimPlayerState)
		{
			BlasterPlayer->BroadcastElim(AttackerPlayerState, VictimPlayerState);
		}
	}
}

void ABattleGameMode::PlayerEliminated(class ABaseCharacter* ElimmedCharacter, class AInGamePlayerController* VictimController, class ABaseAIController* AttackerController)
{
	if (VictimController == nullptr || VictimController->PlayerState == nullptr) return;
	AInGamePlayerState* VictimPlayerState = VictimController ? Cast<AInGamePlayerState>(VictimController->PlayerState) : nullptr;

	ABattleGameState* BattleGameState = GetGameState<ABattleGameState>();

	if (ElimmedCharacter)
	{
		ElimmedCharacter->Elim(false);
	}

	//HUD에 누가 죽였는지 띄어주는 UI 코드
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		AInGamePlayerController* BlasterPlayer = Cast<AInGamePlayerController>(*It);
		//if (BlasterPlayer && AttackerPlayerState && VictimPlayerState)
		//{
		//	BlasterPlayer->BroadcastElim(AttackerPlayerState, VictimPlayerState);
		//}
	}
}

void ABattleGameMode::EnemyEliminated(class ABaseEnemyCharacter* ElimmedCharacter, class ABaseAIController* VictimController, class AInGamePlayerController* AttackerController)
{
	if (AttackerController == nullptr || AttackerController->PlayerState == nullptr) return;
	AInGamePlayerState* AttackerPlayerState = AttackerController ? Cast<AInGamePlayerState>(AttackerController->PlayerState) : nullptr;

	ABattleGameState* BattleGameState = GetGameState<ABattleGameState>();

	if (AttackerPlayerState && BattleGameState)
	{
		AttackerPlayerState->AddToScore(1.f);
	}

	if (ElimmedCharacter)
	{
		ElimmedCharacter->Elim(false);
	}

	//HUD에 누가 죽였는지 띄어주는 UI 코드
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		AInGamePlayerController* BlasterPlayer = Cast<AInGamePlayerController>(*It);
		//if (BlasterPlayer && AttackerPlayerState && VictimPlayerState)
		//{
		//	BlasterPlayer->BroadcastElim(AttackerPlayerState, VictimPlayerState);
		//}
	}
}

void ABattleGameMode::RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController)
{
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Reset();
		ElimmedCharacter->Destroy();
	}
	if (ElimmedController)
	{
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
		int32 Selection = FMath::RandRange(0, PlayerStarts.Num() - 1);
		RestartPlayerAtPlayerStart(ElimmedController, PlayerStarts[Selection]);
	}
}

void ABattleGameMode::PlayerLeftGame(AInGamePlayerState* PlayerLeaving)
{
	if (PlayerLeaving == nullptr) return;
	ABattleGameState* BattleGameState = GetGameState<ABattleGameState>();
	//if (BlasterGameState && BlasterGameState->TopScoringPlayers.Contains(PlayerLeaving))
	//{
	//	BlasterGameState->TopScoringPlayers.Remove(PlayerLeaving);
	//}
	ABaseCharacter* CharacterLeaving = Cast<ABaseCharacter>(PlayerLeaving->GetPawn());
	if (CharacterLeaving)
	{
		CharacterLeaving->Elim(true);
	}
}