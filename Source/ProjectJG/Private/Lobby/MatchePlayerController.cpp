#include "Lobby/MatchePlayerController.h"
#include "Global.h"
#include "Engine.h"
#include "GameFramework/PlayerState.h"
#include "Engine/Player.h"
#include "Lobby/MatcheLobbyUserWidget.h"
#include "Lobby/MatcheLobbyItemUserWidget.h"
#include "Net/UnrealNetwork.h"
#include "Lobby/MatcheGameMode.h"
#include "Lobby/MatcheLobbyGameState.h"
#include "Components/TextBlock.h"

void AMatchePlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(AMatchePlayerController, SlotIdx);
	//DOREPLIFETIME(AMatchePlayerController, bReady);
}

void AMatchePlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	/*if (PlayerState)
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, PlayerState->GetPlayerName());*/
}

void AMatchePlayerController::Tick(float DeltaTime)
{
	/*if (PlayerState)
	{
		if (MatcheMenuItem)
			MatcheMenuItem->UpdatePing(FString::SanitizeFloat(PlayerState->ExactPing));
	}*/
	//if (MatcheLobbyGameState)
		//MatcheLobbyGameState->UpdatePlayerControllers();
}

void AMatchePlayerController::BeginDestroy()
{
	//GEngine->AddOnScreenDebugMessage(-1, 200.0f, FColor::Red, TEXT("BeginDestroyMatchePlayerController"));

	Super::BeginDestroy();
}

void AMatchePlayerController::CheckData()
{
	MatcheLobbyGameState = GetWorld()->GetGameState<AMatcheLobbyGameState>();
	PlayerState = GetPlayerState<APlayerState>();
	//GEngine->AddOnScreenDebugMessage(3, 200.0f, FColor::Purple, PlayerState->GetPlayerName());
	//GEngine->AddOnScreenDebugMessage(4, 2.0f, FColor::Blue, TEXT("CheckData"));
	if (MatcheLobbyGameState&& PlayerState)
	{
		if (IsLocalPlayerController())
		{
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("IsLocalController"));
			if (MatcheMenuClass)
			{
				MatcheMenu = CreateWidget<UMatcheLobbyUserWidget>(this, MatcheMenuClass);
				if (MatcheMenu)
				{
					MatcheMenu->AddToViewport();
					MatcheMenu->SetVisibility(ESlateVisibility::Visible);

					MatcheMenu->StartTextBlock->SetText(FText::FromString(HasAuthority() ? "Start" : "Ready"));

					//if (MatcheLobbyGameState)
					MatcheLobbyGameState->MatcheMenu = MatcheMenu;
					//MatcheMenu->MenuSetup(1, "FreeForAll", "/Game/Developers/JJS/TestMap/MultiLobby/TestMap");
				}
			}
		}
		MatcheLobbyGameState->UpdateTest();
		GetWorldTimerManager().ClearTimer(PlayerCheckDataTimerHandle);
	}
}

void AMatchePlayerController::JoinMatcheLobby_Implementation()
{
	GetWorldTimerManager().SetTimer(PlayerCheckDataTimerHandle, this, &AMatchePlayerController::CheckData, 0.1f, true);
}

void AMatchePlayerController::LogoutMatcheLobby_Implementation()
{
}


void AMatchePlayerController::ClickStartButton_Implementation(bool bReady)
{
	MatcheLobbyGameState = GetWorld()->GetGameState<AMatcheLobbyGameState>();
	PlayerState = GetPlayerState<APlayerState>();
	if (IsLocalPlayerController())
	{
		AMatcheGameMode* MatcheGameMode = GetWorld()->GetAuthGameMode<AMatcheGameMode>();
		
		if(MatcheGameMode)
			if (bReady)
			{
				for (int32 i = 1; i < MatcheLobbyGameState->PlayerReadys.Num(); ++i)
				{
					if (!MatcheLobbyGameState->PlayerReadys[i])
						return;
				}
				if (MatcheMenu)
					MatcheMenu->MenuTearDown();
				MatcheGameMode->TravelToMap("");
			}
	}
	else
	{
		if (PlayerState)
		{
			if (MatcheLobbyGameState)
			{
				int32 idx = MatcheLobbyGameState->PlayerInfos.Find(PlayerState->GetPlayerId());
				if (idx == INDEX_NONE)
					return;
				MatcheLobbyGameState->PlayerReadys[idx] = bReady;
				MatcheLobbyGameState->OnRep_PlayerReadys();
			}
		}
	}
}

void AMatchePlayerController::CreateMatcheWidget_Implementation()
{
	//MatcheLobbyGameState = GetWorld()->GetGameState<AMatcheLobbyGameState>();
	//if(MatcheLobbyGameState)
	//	MatcheLobbyGameState->UpdatePlayerControllers();
	//GEngine->AddOnScreenDebugMessage(5, 20.0f, FColor::Purple, TEXT("CreateMatcheWidgetClient"));
	//if(PlayerState)
	//	//GEngine->AddOnScreenDebugMessage(SlotIdx, 12.0f, FColor::Yellow, PlayerState->GetPlayerName());
	//else
	//{
	//	PlayerState = GetPlayerState<APlayerState>();
	//}
	
}

void AMatchePlayerController::DeleteMatcheWidget_Implementation()
{
	if(MatcheMenu)
		MatcheMenu->MenuTearDown();
}

//void AMatchePlayerController::OnRep_SlotIdx(int32 LastIdx)
//{
//	//if (GEngine)
//	//{
//	//	FString str = "";
//	//	str.AppendInt(SlotIdx);
//	//	GEngine->AddOnScreenDebugMessage(-2, 20.0f, FColor::Green, str);
//	//}
//	//GEngine->AddOnScreenDebugMessage(-2, 20.0f, FColor::Black, TEXT("OnRep_SlotIdx"));
//	//JoinMatcheLobby();
//	MatcheLobbyGameState = GetWorld()->GetGameState<AMatcheLobbyGameState>();
//	if(MatcheLobbyGameState)
//		MatcheLobbyGameState->UpdatePlayerControllers();
//}

//void AMatchePlayerController::OnRep_Ready(bool LastIsReady)
//{
//	if (MatcheMenu == nullptr)
//		return;
//	//MatcheMenuItem = MatcheMenu->GetMatchePalyerWidget(SlotIdx);
//	//if (MatcheMenuItem != nullptr)
//	//{
//	//	if (PlayerState)
//	//	{
//	//		MatcheMenuItem->UpdateIsReady(IsReady);
//	//	}
//	//}
//}

void AMatchePlayerController::UpdateMatcheWidget()
{
	//if (MatcheMenu)
	//	MatcheMenuItem = MatcheMenu->GetMatchePalyerWidget(SlotIdx);
	//if (MatcheMenuItem)
	//{
	//	MatcheMenuItem->SetVisibility(ESlateVisibility::Visible);
	//	if (PlayerState)
	//	{
	//		//GEngine->AddOnScreenDebugMessage(4, 2.0f, FColor::Red, TEXT("UpdateMatcheWidget"));
	//		//GEngine->AddOnScreenDebugMessage(5, 2.0f, FColor::Red, PlayerState->GetPlayerName());
	//		MatcheMenuItem->UpdatePlayerName(PlayerState->GetPlayerName());
	//		MatcheMenuItem->UpdatePing(FString::SanitizeFloat(PlayerState->ExactPing));
	//	}
	//}
}