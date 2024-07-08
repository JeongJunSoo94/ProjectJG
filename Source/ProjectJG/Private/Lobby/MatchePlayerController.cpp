#include "Lobby/MatchePlayerController.h"
#include "Global.h"
#include "Engine.h"
#include "GameFramework/PlayerState.h"
#include "Engine/Player.h"
#include "MatcheLobbyUserWidget.h"
#include "Net/UnrealNetwork.h"
#include "MatcheLobbyItemUserWidget.h"
#include "Lobby/MatcheGameMode.h"
#include "Lobby/MatcheLobbyGameState.h"

void AMatchePlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMatchePlayerController, SlotIdx);
	DOREPLIFETIME(AMatchePlayerController, IsReady);
}

void AMatchePlayerController::BeginPlay()
{
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(-2, 10.0f, FColor::Green, TEXT("BeginPlay"));
	MatcheLobbyGameState = GetWorld()->GetGameState<AMatcheLobbyGameState>();
	if(MatcheLobbyGameState)
		MatcheLobbyGameState->PlayerControllers.Add(this);
	if (IsLocalController())
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green,TEXT("IsLocalController"));
		if (MatcheMenuClass)
		{
			MatcheMenu = CreateWidget<UMatcheLobbyUserWidget>(this, MatcheMenuClass);
			if (MatcheMenu)
			{
				MatcheMenu->AddToViewport();
				MatcheMenu->SetVisibility(ESlateVisibility::Visible);
				if(MatcheLobbyGameState)
					MatcheLobbyGameState->MatcheMenu = MatcheMenu;
				//MatcheMenu->MenuSetup(1, "FreeForAll", "/Game/Developers/JJS/TestMap/MultiLobby/TestMap");
			}
		}
	}
	else
	{
		if(MatcheLobbyGameState)
			MatcheMenu = MatcheLobbyGameState->MatcheMenu;
	}
	JoinMatcheLobby();
	IsReady = false;
	PlayerState = GetPlayerState<APlayerState>();
	/*if (PlayerState)
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, PlayerState->GetPlayerName());*/
}

void AMatchePlayerController::Tick(float DeltaTime)
{
	if (IsLocalController())
		GEngine->AddOnScreenDebugMessage(3, 2.0f, FColor::Green, TEXT("Tick"));
	if (PlayerState)
	{
		if (MatcheMenuItem)
			MatcheMenuItem->UpdatePing(FString::SanitizeFloat(PlayerState->ExactPing));
	}
}

void AMatchePlayerController::BeginDestroy()
{
	Super::BeginDestroy();
	if (MatcheMenu)
	{
		MatcheMenu->RemoveFromViewport();
		MatcheMenu = nullptr;
	}
}

void AMatchePlayerController::JoinMatcheLobby_Implementation()
{
	AMatcheGameMode* GameMode = Cast<AMatcheGameMode>(GetWorld()->GetAuthGameMode());
	if(GameMode)
		SlotIdx = GameMode->GetSlotIdx(this);
	UpdateMatcheWidget();
}

void AMatchePlayerController::LogoutMatcheLobby_Implementation()
{
	if (MatcheMenu == nullptr)
		return;
	MatcheMenuItem = MatcheMenu->GetMatchePalyerWidget(SlotIdx);
	if (MatcheMenuItem != nullptr)
		MatcheMenuItem->SetVisibility(ESlateVisibility::Hidden);
}

void AMatchePlayerController::UpdateMatcheWidget_Implementation()
{
	GEngine->AddOnScreenDebugMessage(3, 2.0f, FColor::Green, TEXT("UpdateMatcheWidget"));
	if (MatcheMenu)
		MatcheMenuItem = MatcheMenu->GetMatchePalyerWidget(SlotIdx);
	if (MatcheMenuItem)
	{
		MatcheMenuItem->SetVisibility(ESlateVisibility::Visible);
		if (PlayerState)
		{
			MatcheMenuItem->UpdatePlayerName(PlayerState->GetPlayerName());
			MatcheMenuItem->UpdatePing(FString::SanitizeFloat(PlayerState->ExactPing));
		}
	}
}

void AMatchePlayerController::OnRep_Ready(bool LastIsReady)
{
	if (MatcheMenu == nullptr)
		return;
	MatcheMenuItem = MatcheMenu->GetMatchePalyerWidget(SlotIdx);
	if (MatcheMenuItem != nullptr)
	{
		if (PlayerState)
		{
			MatcheMenuItem->UpdateIsReady(IsReady);
		}
	}
}