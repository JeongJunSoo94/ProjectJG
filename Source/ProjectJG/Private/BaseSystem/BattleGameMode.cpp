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
			//플레이어가 모두 참여 했는지 확인
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
	else if (MatchState == MatchState::WaitingPostMatch)
	{
	}
	else if (MatchState == MatchState::Cooldown)
	{
		CountdownTime = CooldownTime + WarmupTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f)
		{
			RestartGame();
			//UWorld* World = GetWorld();
			//if (World)
			//{
			//	//MatcheMenu->MenuSetup(1, "FreeForAll", "/Game/Developers/JJS/TestMap/MultiLobby/TestMap");
			//	World->ServerTravel(FString("/Game/Developers/JJS/TestMap/MultiLobby/MultiLobby?listen"), true);
			//}
		}
	}
	else if (MatchState == MatchState::LeavingMap)
	{
	}
	else if (MatchState == MatchState::Aborted)
	{
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

bool ABattleGameMode::CheckReadyPlayer()
{
	int32 playerNum = GameState.Get()->PlayerArray.Num();
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		AInGamePlayerController* PlayerController = Cast<AInGamePlayerController>(*It);
		if (PlayerController)
		{
			--playerNum;
		}
		else
		{
			APlayerController* tempController = Cast<APlayerController>(*It);
			RestartPlayer(tempController);
			//It->Get()->ClientTravel("/Game/Developers/JJS/TestMap/TestMap?listen", ETravelType::TRAVEL_Relative);
			//APlayerController* OldController = It->Get();
			//if (GEngine)
				//GEngine->AddOnScreenDebugMessage(-1, 12.0f, FColor::Blue, "CheckReadyPlayer");
			//if (!OldController->IsA(AInGamePlayerController::StaticClass()))
			//{
			//	// 기존 컨트롤러를 새 컨트롤러로 변경
			//	APlayerController* NewController = GetWorld()->SpawnActor<AInGamePlayerController>();

			//	// 기존 Pawn 소유권 이전
			//	APawn* OldPawn = OldController->GetPawn();
			//	if (OldPawn)
			//	{
			//		NewController->Possess(OldPawn);
			//	}

			//	// 기존 컨트롤러 제거
			//	OldController->Destroy();
			//}
		}
	}
	if(playerNum==0)
		return true;
	return false;
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
	//if (GEngine)
	//{
	//	FString str = "RequestRespawn:";
	//	//str.AppendInt(GameInstance->GetLocalPlayers().Num());
	//	GEngine->AddOnScreenDebugMessage(4, 20.0f, FColor::Purple, str);
	//}
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
	if (GEngine)
	{
		FString str = "PlayerLeftGame:";
		//str.AppendInt(GameInstance->GetLocalPlayers().Num());
		GEngine->AddOnScreenDebugMessage(5, 20.0f, FColor::Purple, str);
	}
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