// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MatcheLobbyUserWidget.generated.h"

/**
 * 
 */

UCLASS()
class PROJECTJG_API UMatcheLobbyUserWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;
	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

	TArray<class UMatcheLobbyItemUserWidget*> MatcheLobbyItemUserWidgets;

	UPROPERTY(meta = (BindWidget))
		class UButton* StartButton;

	UPROPERTY(meta = (BindWidget))
		UButton* LeaveButton;

	UPROPERTY(meta = (BindWidget))
		UButton* CharacterSelectButton;

	UPROPERTY(meta = (BindWidget))
		UButton* FriendButton;

	FString PathToMainMenu{ TEXT("/Game/Developers/JJS/TestMap/MultiLobby/MultiStartMenu?listen") };

	class AMatchePlayerController* MatchePlayerController;

public:
	void MenuTearDown();
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* StartTextBlock;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* MatcheName;

	UPROPERTY(meta = (BindWidget))
		class UVerticalBox* MatchesVerticalBox;

	UMatcheLobbyItemUserWidget* GetMatchePalyerWidget(int32 idx);

	UFUNCTION()
		void StartButtonClicked();

	UFUNCTION()
		void LeaveButtonClicked();

	UFUNCTION()
		void CharacterSelectButtonClicked();

	UFUNCTION()
		void FriendButtonClicked();

};
