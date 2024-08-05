// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyPlayerController.h"
#include "Global.h"
#include "Engine.h"
#include "GameFramework/PlayerState.h"
#include "Engine/Player.h"
#include "MenuUserWidget.h"

ALobbyPlayerController::ALobbyPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//ShouldShowMouseCursor();
	SetShowMouseCursor(true);

}

void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (MainMenu)
	{
		MainMenu = CreateWidget<UMenuUserWidget>(this, MainMenuClass);
		if (MainMenu)
		{
			//MainMenu->AddToViewport();
			MainMenu->MenuSetup(1,"FreeForAll","/Game/Developers/JJS/TestMap/MultiLobby/MultiLobby");
		}
	}

	//APlayerState* player = GetPlayerState<APlayerState>();
	//if (player)
	//	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, player->GetPlayerName());
}

void ALobbyPlayerController::BeginDestroy()
{
	Super::BeginDestroy();
	if (MainMenu)
	{
		MainMenu->RemoveFromViewport();
		MainMenu = nullptr;
	}
}