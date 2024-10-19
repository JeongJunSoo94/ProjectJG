// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MatcheLobbyItemUserWidget.generated.h"

DECLARE_DELEGATE_OneParam(FKickButtonClickedDelegate, int32);
/**
 * 
 */
UCLASS()
class PROJECTJG_API UMatcheLobbyItemUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	int32 slotidx = 0;

	FKickButtonClickedDelegate KickButtonClickedDelegate;
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* PlayerNameText;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ReadyText;
	UPROPERTY(meta = (BindWidget))
		class UButton* KickButton;
public:
	void UpdatePlayerName(FString PlayerName);
	void UpdateIsReady(bool IsReady);
	void SetKickButton(bool OnButton);
	UFUNCTION()
	void KickButtonClicked();
};
