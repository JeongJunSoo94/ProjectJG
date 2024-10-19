// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FriendsInviteUserWidget.generated.h"

DECLARE_DELEGATE_OneParam(FInviteButtonClickedDelegate, int32);
/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UFriendsInviteUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerNameText;

	UPROPERTY(meta = (BindWidget))
	class UImage* AvatarImage;

	UPROPERTY(meta = (BindWidget))
	class UButton* InviteButton;

	int32 slotidx = 0;

	FInviteButtonClickedDelegate InviteButtonClickedDelegate;

	void UpdateData(FString PlayerName, UTexture2D* Texture, bool bOnline);
	UFUNCTION()
		void InviteButtonClicked();

	void Initialized();
};
