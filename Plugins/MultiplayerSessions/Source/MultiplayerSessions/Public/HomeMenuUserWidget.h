// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HomeMenuUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UHomeMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
		class UButton* StartButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* MultiButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* QuitButton;

	class UMenuUserWidget* MenuUser;
protected:
	virtual void NativeConstruct() override;
	UFUNCTION()
		void StartButtonClicked();
	UFUNCTION()
		void MultiButtonClicked();
	UFUNCTION()
		void QuitButtonClicked();
};
