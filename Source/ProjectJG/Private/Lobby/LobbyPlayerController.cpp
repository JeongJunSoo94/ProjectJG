// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyPlayerController.h"
#include "Global.h"

ALobbyPlayerController::ALobbyPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//ShouldShowMouseCursor();
	SetShowMouseCursor(true);
}
