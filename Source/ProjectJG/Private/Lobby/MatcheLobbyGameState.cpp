#include "Lobby/MatcheLobbyGameState.h"
#include "MultiplayerSessionsSubsystem.h"
#include "Lobby/MatchePlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Lobby/MatcheGameMode.h"
#include "Net/UnrealNetwork.h"
#include "Lobby/MatcheLobbyUserWidget.h"
#include "Lobby/MatcheLobbyItemUserWidget.h"

void AMatcheLobbyGameState::BeginPlay()
{
}

void AMatcheLobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMatcheLobbyGameState, PlayersInfo);
	DOREPLIFETIME(AMatcheLobbyGameState, PlayerReadys);
}

void AMatcheLobbyGameState::UpdateMatcheItems()
{
	for (int32 i = 0; i < PlayerArray.Num(); ++i)
	{
		if (PlayerArray[i])
		{
			if (MatcheMenu)
			{
				int32 idx = PlayersInfo.Find(PlayerArray[i]->GetPlayerId());
				if (idx == INDEX_NONE)
					continue;
				UMatcheLobbyItemUserWidget* MatcheMenuItem = MatcheMenu->GetMatchePalyerWidget(idx);
				if (MatcheMenuItem)
				{
					MatcheMenuItem->SetVisibility(ESlateVisibility::Visible);
					MatcheMenuItem->SetKickButton(i==0?false:HasAuthority());
					MatcheMenuItem->UpdatePlayerName(PlayerArray[i]->GetPlayerName());
					//MatcheMenuItem->UpdatePing(FString::SanitizeFloat(PlayerArray[i]->ExactPing));
				}
			}
		}
	}
	for (int32 i = 0; i < PlayersInfo.Num(); ++i)
	{
		if (PlayersInfo[i] == -1)
		{
			if (MatcheMenu)
			{
				UMatcheLobbyItemUserWidget* MatcheMenuItem = MatcheMenu->GetMatchePalyerWidget(i);
				if (MatcheMenuItem)
				{
					MatcheMenuItem->SetVisibility(ESlateVisibility::Hidden);
					MatcheMenuItem->UpdatePlayerName("");
				}
			}
		}
	}
	GetWorldTimerManager().ClearTimer(PlayerCheckDataTimerHandle);
}

void AMatcheLobbyGameState::OnRep_PlayersInfo()
{
	GetWorldTimerManager().SetTimer(PlayerCheckDataTimerHandle, this, &AMatcheLobbyGameState::UpdateMatcheItems, 0.1f, true);
}

void AMatcheLobbyGameState::OnRep_PlayerReadys()
{
	for (int32 i = 0; i < PlayerReadys.Num(); ++i)
	{
		if (MatcheMenu)
		{
			UMatcheLobbyItemUserWidget* MatcheMenuItem = MatcheMenu->GetMatchePalyerWidget(i);
			if (MatcheMenuItem)
			{
				MatcheMenuItem->UpdateIsReady(PlayerReadys[i]);
			}
		}
	}
}
void AMatcheLobbyGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);
	GetWorldTimerManager().SetTimer(PlayerCheckDataTimerHandle, this, &AMatcheLobbyGameState::UpdateMatcheItems, 0.1f, true);
}

void AMatcheLobbyGameState::RemovePlayerState(APlayerState* PlayerState)
{
	Super::RemovePlayerState(PlayerState);
	GetWorldTimerManager().SetTimer(PlayerCheckDataTimerHandle, this, &AMatcheLobbyGameState::UpdateMatcheItems, 0.1f, true);
}

APlayerState* AMatcheLobbyGameState::GetFindPlayerState(int32 SlotIdx)
{
	for (int32 i = 0; i < PlayerArray.Num(); ++i)
	{
		if (PlayerArray[i]->GetPlayerId() == PlayersInfo[SlotIdx])
		{
			return PlayerArray[i];
		}
	}

	return nullptr;
}
