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

void AMatcheLobbyGameState::UpdatePlayerControllers()
{
	//for (int32 i=0;i< PlayerArray.Num();++i)
	//{
	//	if (PlayerArray[i])
	//	{
	//		if (MatcheMenu)
	//		{
	//			UMatcheLobbyItemUserWidget* MatcheMenuItem = MatcheMenu->GetMatchePalyerWidget(i);
	//			if (MatcheMenuItem)
	//			{
	//				MatcheMenuItem->SetVisibility(ESlateVisibility::Visible);
	//				MatcheMenuItem->UpdatePlayerName(PlayerArray[i]->GetPlayerName());
	//				MatcheMenuItem->UpdatePing(FString::SanitizeFloat(PlayerArray[i]->ExactPing));
	//			}
	//			else
	//			{
	//				//GEngine->AddOnScreenDebugMessage(4, 20.0f, FColor::Red, TEXT("NotMatcheMenuItem"));
	//			}
	//		}
	//		else
	//		{
	//			//GEngine->AddOnScreenDebugMessage(4, 20.0f, FColor::Red, TEXT("NotMatcheMenu"));
	//		}
	//	}
	//	else
	//	{
	//		if (GEngine)
	//		{
	//			FString str = "";
	//			str.AppendInt(i);
	//			GEngine->AddOnScreenDebugMessage(i+10, 2.0f, FColor::Yellow, str);
	//		}
	//	}
	//}
}

void AMatcheLobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMatcheLobbyGameState, PlayerInfos);
	DOREPLIFETIME(AMatcheLobbyGameState, PlayerReadys);
}

void AMatcheLobbyGameState::UpdateTest()
{
	for (int32 i = 0; i < PlayerArray.Num(); ++i)
	{
		if (PlayerArray[i])
		{
			if (MatcheMenu)
			{
				int32 idx = PlayerInfos.Find(PlayerArray[i]->GetPlayerId());
				if (idx == INDEX_NONE)
					continue;
				UMatcheLobbyItemUserWidget* MatcheMenuItem = MatcheMenu->GetMatchePalyerWidget(idx);
				if (MatcheMenuItem)
				{
					MatcheMenuItem->SetVisibility(ESlateVisibility::Visible);
					MatcheMenuItem->SetKickButton(HasAuthority());
					MatcheMenuItem->UpdatePlayerName(PlayerArray[i]->GetPlayerName());
					MatcheMenuItem->UpdatePing(FString::SanitizeFloat(PlayerArray[i]->ExactPing));
				}
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(i, 20.0f, FColor::Red, TEXT("NoPlayer"));
			return;
		}
	}
	for (int32 i = 0; i < PlayerInfos.Num(); ++i)
	{
		if (PlayerInfos[i] == -1)
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
			else
				return;
		}
	}
	GetWorldTimerManager().ClearTimer(PlayerCheckDataTimerHandle);
}

void AMatcheLobbyGameState::OnRep_PlayerInfos()
{
	GetWorldTimerManager().SetTimer(PlayerCheckDataTimerHandle, this, &AMatcheLobbyGameState::UpdateTest, 0.1f, true);
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
	GetWorldTimerManager().SetTimer(PlayerCheckDataTimerHandle, this, &AMatcheLobbyGameState::UpdateTest, 0.1f, true);
}

void AMatcheLobbyGameState::RemovePlayerState(APlayerState* PlayerState)
{
	Super::RemovePlayerState(PlayerState);
	GetWorldTimerManager().SetTimer(PlayerCheckDataTimerHandle, this, &AMatcheLobbyGameState::UpdateTest, 0.1f, true);
}