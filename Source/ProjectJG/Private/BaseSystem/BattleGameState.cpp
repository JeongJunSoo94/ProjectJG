#include "BaseSystem/BattleGameState.h"
#include "Global.h"
#include "Net/UnrealNetwork.h"
#include "BaseSystem/PlayerState/InGamePlayerState.h"
#include "BaseSystem/InGamePlayerController.h"

void ABattleGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABattleGameState, TopScoringPlayers);
	DOREPLIFETIME(ABattleGameState, RedTeamScore);
	DOREPLIFETIME(ABattleGameState, BlueTeamScore);
}

void ABattleGameState::UpdateTopScore(class AInGamePlayerState* ScoringPlayer)
{
	if (TopScoringPlayers.Num() == 0)
	{
		TopScoringPlayers.Add(ScoringPlayer);
		TopScore = ScoringPlayer->GetScore();
	}
	else if (ScoringPlayer->GetScore() == TopScore)
	{
		TopScoringPlayers.AddUnique(ScoringPlayer);
	}
	else if (ScoringPlayer->GetScore() > TopScore)
	{
		TopScoringPlayers.Empty();
		TopScoringPlayers.AddUnique(ScoringPlayer);
		TopScore = ScoringPlayer->GetScore();
	}
}

void ABattleGameState::RedTeamScores()
{
	++RedTeamScore;
	AInGamePlayerController* BPlayer = Cast<AInGamePlayerController>(GetWorld()->GetFirstPlayerController());
	if (BPlayer)
	{
		BPlayer->SetHUDRedTeamScore(RedTeamScore);
	}
}

void ABattleGameState::BlueTeamScores()
{
	++BlueTeamScore;
	AInGamePlayerController* BPlayer = Cast<AInGamePlayerController>(GetWorld()->GetFirstPlayerController());
	if (BPlayer)
	{
		BPlayer->SetHUDBlueTeamScore(BlueTeamScore);
	}
}

void ABattleGameState::OnRep_RedTeamScore()
{
	AInGamePlayerController* BPlayer = Cast<AInGamePlayerController>(GetWorld()->GetFirstPlayerController());
	if (BPlayer)
	{
		BPlayer->SetHUDRedTeamScore(RedTeamScore);
	}
}

void ABattleGameState::OnRep_BlueTeamScore()
{
	AInGamePlayerController* BPlayer = Cast<AInGamePlayerController>(GetWorld()->GetFirstPlayerController());
	if (BPlayer)
	{
		BPlayer->SetHUDBlueTeamScore(BlueTeamScore);
	}
}
