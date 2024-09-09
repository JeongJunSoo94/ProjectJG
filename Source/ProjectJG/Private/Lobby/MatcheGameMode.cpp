#include "Lobby/MatcheGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "MultiplayerSessionsSubsystem.h"
#include "Lobby/MatchePlayerController.h"
#include "GameFramework/PlayerState.h"
#include "MultiplayerSessionsSubsystem.h"
#include "Lobby/MatcheLobbyGameState.h"
#include "Engine.h"

void AMatcheGameMode::BeginPlay()
{
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}
	bUseSeamlessTravel = true;
}

void AMatcheGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	AMatchePlayerController* MatchePlayerController=Cast<AMatchePlayerController>(NewPlayer);
	APlayerState* PlayerState = MatchePlayerController->GetPlayerState<APlayerState>();
	if (MatcheLobbyGameState == nullptr)
	{
		//if (GEngine){GEngine->AddOnScreenDebugMessage(-1,15.f,FColor::Red,FString(TEXT("MatcheLobbyGameState!")));}
		MatcheLobbyGameState = GetWorld()->GetGameState<AMatcheLobbyGameState>();
	}
	//if (PlayerState)
	//{
	//	if (GEngine)
	//	{
	//		GEngine->AddOnScreenDebugMessage(
	//			-1,
	//			15.f,
	//			FColor::Green,
	//			FString(TEXT("PostLogin PlayerState!"))
	//		);
	//	}
	//	//GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Blue, PlayerState->GetPlayerName());
	//}
	//else
	//{
	//	if (GEngine)
	//	{
	//		GEngine->AddOnScreenDebugMessage(
	//			-1,
	//			15.f,
	//			FColor::Green,
	//			FString(TEXT("PostLogin Not PlayerState!"))
	//		);
	//	}
	//}

	//플레이어에게 슬롯위치를 부여함
	if (MatchePlayerController)
	{
		//if (GEngine)
		//{
		//	GEngine->AddOnScreenDebugMessage(
		//		-1,
		//		15.f,
		//		FColor::Green,
		//		FString(TEXT("MatchePlayerController!"))
		//	);
		//}

		//int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
		//if (MatcheLobbyGameState->PlayerControllers.Num() > NumberOfPlayers)
		//{
		//	int32 Idx = MatcheLobbyGameState->PlayerControllers.Find(nullptr);
		//	//MatchePlayerController->SlotIdx = GameState.Get()->PlayerArray.Num();
		//	MatcheLobbyGameState->PlayerControllers[Idx] = MatchePlayerController;
		//}
		//else
		//{
		//	//MatchePlayerController->SlotIdx = NumberOfPlayers - 1;
		//	MatcheLobbyGameState->PlayerControllers.Add(MatchePlayerController);
		//}
		//FString str;
		//str.Append("Num:");
		//str.AppendInt(MatcheLobbyGameState->PlayerInfos.Num());
		//str.Append("PlayerName:");
		//str.Append(PlayerState->GetPlayerName());
		//str.Append("PlayerId:");
		//str.AppendInt(PlayerState->GetPlayerId());
		//MatchePlayerController->CreateMatcheWidget();
		MatchePlayerController->JoinMatcheLobby();
		int32 idx = MatcheLobbyGameState->PlayerInfos.Find(-1);
		if (idx == INDEX_NONE)
		{
			MatcheLobbyGameState->PlayerInfos.Add(PlayerState->GetPlayerId());
			MatcheLobbyGameState->PlayerReadys.Add(false);
		}
		else
		{
			MatcheLobbyGameState->PlayerInfos[idx] = PlayerState->GetPlayerId();
			MatcheLobbyGameState->PlayerReadys[idx] = false;
		}
		MatcheLobbyGameState->UpdateTest();
		//for (int32 i =0; i< MatcheLobbyGameState->PlayerControllers.Num();++i)
		//{
		//	MatcheLobbyGameState->PlayerControllers[i]->SlotIdx = i;
		//	MatcheLobbyGameState->PlayerControllers[i]->CreateMatcheWidget();
		//	//GEngine->AddOnScreenDebugMessage(i, 12.0f, FColor::Red, MatcheLobbyGameState->PlayerControllers[i]->GetPlayerState<APlayerState>()->GetPlayerName());
		//}
		//새로 친구가 들어왔어 갱신해
		//GameState.Get()->PlayerArray
		//MatchePlayerController->SlotIdx = .Num()-1;
		//MatchePlayerController->CreateMatcheWidget();
		/*if (HasAuthority())
			MatchePlayerController->JoinMatcheLobby();*/
		//if (HasAuthority())
		//	MatchePlayerController->OnRep_SlotIdx();
		//MatcheLobbyGameState->UpdatePlayerControllers();
	}
}

void AMatcheGameMode::Logout(AController* Exiting)
{
	AMatchePlayerController* MatchePlayerController = Cast<AMatchePlayerController>(Exiting);
	if (MatcheLobbyGameState)
	{
		int32 idx = MatcheLobbyGameState->PlayerInfos.Find(MatchePlayerController->GetPlayerState<APlayerState>()->GetPlayerId());
		MatcheLobbyGameState->PlayerInfos[idx] = -1;
		MatcheLobbyGameState->PlayerReadys[idx] = false;
	}
	if (MatchePlayerController)
	{
		MatchePlayerController->LogoutMatcheLobby();
	}
	Super::Logout(Exiting);
}

void AMatcheGameMode::TravelToMap(const FString& MapName)
{
	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UMultiplayerSessionsSubsystem* Subsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
		check(Subsystem);

		if (NumberOfPlayers == Subsystem->DesiredNumPublicConnections)
		{
			UWorld* World = GetWorld();
			if (World)
			{
				PlayersWidgetDelete();
				bUseSeamlessTravel = true;

				FString MatchType = Subsystem->DesiredMatchType;
				if (MatchType == "FreeForAll")
				{
					World->ServerTravel(FString("/Game/Developers/JJS/TestMap/TestMap?listen"),true);
					//UGameplayStatics::OpenLevel(World, FName("/Game/Developers/JJS/TestMap/TestMap?listen"), true);
				}
				else if (MatchType == "Teams")
				{
					World->ServerTravel(FString("/Game/Maps/Teams?listen"));
				}
				/*			
				else if (MatchType == "CaptureTheFlag")
				{
					World->ServerTravel(FString("/Game/Maps/CaptureTheFlag?listen"));
				}
				*/
			}
		}
	}
}

int32 AMatcheGameMode::GetSlotIdx(APlayerController* Controller)
{
	int32 Idx = GameState.Get()->PlayerArray.Find(Controller->GetPlayerState<APlayerState>());
	return Idx;
}
int32 AMatcheGameMode::GetSessionPlayerNum() const
{
	return MultiplayerSessionsSubsystem->GetSessionPlayerNum(); 
}

void AMatcheGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (EndPlayReason == EEndPlayReason::LevelTransition)
	{
		UE_LOG(LogTemp, Log, TEXT("Level transitioning..."));
	}
}
void AMatcheGameMode::PlayersWidgetDelete()
{
	if (MatcheLobbyGameState)
	{
		for (int32 i = 0; i < MatcheLobbyGameState->PlayerArray.Num(); ++i)
		{
			AMatchePlayerController* MatchePlayerController = Cast<AMatchePlayerController>(MatcheLobbyGameState->PlayerArray[i]->GetPlayerController());
			if (MatchePlayerController)
			{
				MatchePlayerController->DeleteMatcheWidget();
			}
		}
	}
}

void AMatcheGameMode::PostSeamlessTravel()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 200.0f, FColor::Red, TEXT("PostSeamlessTravel"));
	UWorld* World = GetWorld();
	if (World)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(2, 200.0f, FColor::Red, World->GetMapName());
	}
	Super::PostSeamlessTravel();
	World = GetWorld();
	if (World)
	{
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(3, 200.0f, FColor::Blue, World->GetMapName());
	}
}