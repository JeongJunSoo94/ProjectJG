#include "Lobby/MatcheGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "MultiplayerSessionsSubsystem.h"
#include "Lobby/MatchePlayerController.h"
#include "GameFramework/PlayerState.h"
#include "MultiplayerSessionsSubsystem.h"

void AMatcheGameMode::BeginPlay()
{
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}
}

void AMatcheGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	AMatchePlayerController* MatchePlayerController=Cast<AMatchePlayerController>(NewPlayer);
	APlayerState* PlayerState = MatchePlayerController->GetPlayerState<APlayerState>();
	if (PlayerState)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Green,
				FString(TEXT("Success to create session!"))
			);
		}
	}

	if (MatchePlayerController)
	{
		int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
		if (PlayerControllers.Num() > NumberOfPlayers)
		{
			int32 Idx = PlayerControllers.Find(nullptr);
			MatchePlayerController->SlotIdx = Idx;
			PlayerControllers[Idx] = MatchePlayerController;
		}
		else
		{
			MatchePlayerController->SlotIdx = NumberOfPlayers-1;
			PlayerControllers.Add(MatchePlayerController);
		}
	}
}

void AMatcheGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	AMatchePlayerController* MatchePlayerController = Cast<AMatchePlayerController>(Exiting);

	if (MatchePlayerController)
	{
		MatchePlayerController->LogoutMatcheLobby();
		int32 Idx = PlayerControllers.Find(MatchePlayerController);
		PlayerControllers[Idx] = nullptr;
		MatchePlayerController->SlotIdx = -1;
	}
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
				bUseSeamlessTravel = true;

				FString MatchType = Subsystem->DesiredMatchType;
				if (MatchType == "FreeForAll")
				{
					World->ServerTravel(FString("/Game/Developers/JJS/TestMap/TestMap?listen"));
				}
				else if (MatchType == "Teams")
				{
					World->ServerTravel(FString("/Game/Maps/Teams?listen"));
				}
				/*			
							else if (MatchType == "CaptureTheFlag")
							{
								World->ServerTravel(FString("/Game/Maps/CaptureTheFlag?listen"));
							}*/
			}
		}
	}
}

int32 AMatcheGameMode::GetSlotIdx(APlayerController* Controller)
{
	int32 Idx = PlayerControllers.Find(Controller);
	return Idx;
}
int32 AMatcheGameMode::GetSessionPlayerNum() const
{
	return MultiplayerSessionsSubsystem->GetSessionPlayerNum(); 
}