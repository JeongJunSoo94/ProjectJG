#include "BaseSystem/BattleGameMode/TeamsBattleGameMode.h"
#include "Global.h"
#include "GameFramework/PlayerStart.h"
#include "BaseSystem/InGamePlayerController.h"
#include "BaseSystem/PlayerState/InGamePlayerState.h"
#include "BaseSystem/BattleGameState.h"
#include "Character/BaseCharacter.h"

ATeamsBattleGameMode::ATeamsBattleGameMode()
{
	bTeamsMatch = true;
}

void ATeamsBattleGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ABattleGameState* BGameState = Cast<ABattleGameState>(UGameplayStatics::GetGameState(this));
	if (BGameState)
	{
		AInGamePlayerState* BPState = NewPlayer->GetPlayerState<AInGamePlayerState>();
		if (BPState && BPState->GetTeam() == ETeam::ET_NoTeam)
		{
			if (BGameState->BlueTeam.Num() >= BGameState->RedTeam.Num())
			{
				BGameState->RedTeam.AddUnique(BPState);
				BPState->SetTeam(ETeam::ET_RedTeam);
			}
			else
			{
				BGameState->BlueTeam.AddUnique(BPState);
				BPState->SetTeam(ETeam::ET_BlueTeam);
			}
		}
	}
}

void ATeamsBattleGameMode::Logout(AController* Exiting)
{
	ABattleGameState* BGameState = Cast<ABattleGameState>(UGameplayStatics::GetGameState(this));
	AInGamePlayerState* BPState = Exiting->GetPlayerState<AInGamePlayerState>();
	if (BGameState && BPState)
	{
		if (BGameState->RedTeam.Contains(BPState))
		{
			BGameState->RedTeam.Remove(BPState);
		}
		if (BGameState->BlueTeam.Contains(BPState))
		{
			BGameState->BlueTeam.Remove(BPState);
		}
	}

}

void ATeamsBattleGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();

	ABattleGameState* BGameState = Cast<ABattleGameState>(UGameplayStatics::GetGameState(this));
	if (BGameState)
	{
		for (auto PState : BGameState->PlayerArray)
		{
			AInGamePlayerState* BPState = Cast<AInGamePlayerState>(PState.Get());
			if (BPState && BPState->GetTeam() == ETeam::ET_NoTeam)
			{
				if (BGameState->BlueTeam.Num() >= BGameState->RedTeam.Num())
				{
					BGameState->RedTeam.AddUnique(BPState);
					BPState->SetTeam(ETeam::ET_RedTeam);
				}
				else
				{
					BGameState->BlueTeam.AddUnique(BPState);
					BPState->SetTeam(ETeam::ET_BlueTeam);
				}
			}
		}
	}
}

float ATeamsBattleGameMode::CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage)
{
	AInGamePlayerState* AttackerPState = Attacker->GetPlayerState<AInGamePlayerState>();
	AInGamePlayerState* VictimPState = Victim->GetPlayerState<AInGamePlayerState>();
	if (AttackerPState == nullptr || VictimPState == nullptr) return BaseDamage;
	if (VictimPState == AttackerPState)
	{
		return BaseDamage;
	}
	if (AttackerPState->GetTeam() == VictimPState->GetTeam())
	{
		return 0.f;
	}
	return BaseDamage;
}

void ATeamsBattleGameMode::PlayerEliminated(ABaseCharacter* ElimmedCharacter, AInGamePlayerController* VictimController, AInGamePlayerController* AttackerController)
{
	Super::PlayerEliminated(ElimmedCharacter, VictimController, AttackerController);

	ABattleGameState* BGameState = Cast<ABattleGameState>(UGameplayStatics::GetGameState(this));
	AInGamePlayerState* AttackerPlayerState = AttackerController ? Cast<AInGamePlayerState>(AttackerController->PlayerState) : nullptr;
	if (BGameState && AttackerPlayerState)
	{
		if (AttackerPlayerState->GetTeam() == ETeam::ET_BlueTeam)
		{
			BGameState->BlueTeamScores();
		}
		if (AttackerPlayerState->GetTeam() == ETeam::ET_RedTeam)
		{
			BGameState->RedTeamScores();
		}
	}
}