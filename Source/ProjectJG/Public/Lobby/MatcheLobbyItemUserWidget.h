// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MatcheLobbyItemUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTJG_API UMatcheLobbyItemUserWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* PlayerNameText;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ReadyText;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* PingText;
	UPROPERTY(meta = (BindWidget))
		class UButton* MatcheSelectButton;
public:
	void UpdatePlayerName(FString PlayerName);
	void UpdateIsReady(bool IsReady);
	void UpdatePing(FString Ping);
	void SetKickButton(bool OnButton);
};
