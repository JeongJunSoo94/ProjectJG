// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuUserWidget.generated.h"

UCLASS()
class MULTIPLAYERSESSIONS_API UPauseMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
		void ContinueButtonClicked();
	UFUNCTION()
		void OptionButtonClicked();
	UFUNCTION()
		void LeaveButtonClicked();
	UFUNCTION()
		void QuitButtonClicked();
protected:
	virtual void NativeConstruct() override;
private:
	UPROPERTY(meta = (BindWidget))
		class UButton* ContinueButton;

	UPROPERTY(meta = (BindWidget))
		UButton* OptionButton;

	UPROPERTY(meta = (BindWidget))
		UButton* LeaveButton;

	UPROPERTY(meta = (BindWidget))
		UButton* QuitButton;

	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

	FString PathToMainMenu{ TEXT("/Game/Developers/JJS/TestMap/MultiLobby/MultiStartMenu?listen") };

	APlayerController* PlayerController;
};
