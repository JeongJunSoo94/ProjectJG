// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Announcement.generated.h"

/**
 * 
 */

namespace Announcement
{
	const FString NewMatchStartsIn(TEXT("New match starts in:"));
	const FString ThereIsNoWinner(TEXT("There is no winner."));
	const FString YouAreTheWinner(TEXT("You are the winner!"));
	const FString PlayersTiedForTheWin(TEXT("Players tied for the win:"));
	const FString TeamsTiedForTheWin(TEXT("Teams tied for the win:"));
	const FString RedTeam(TEXT("Red team"));
	const FString BlueTeam(TEXT("Blue team"));
	const FString RedTeamWins(TEXT("Red team wins!"));
	const FString BlueTeamWins(TEXT("Blue team wins!"));
}

UCLASS()
class PROJECTJG_API UAnnouncement : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* WarmupTime;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AnnouncementText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* InfoText;
};
