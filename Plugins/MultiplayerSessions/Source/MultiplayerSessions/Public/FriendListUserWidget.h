// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FriendsInviteUserWidget.h"
#include "FriendListUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UFriendListUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* FriendContainer;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<class UFriendsInviteUserWidget> FriendWidgetClass;

	void ClearFriendList();

	UFriendsInviteUserWidget* AddChildWidget(FString PlayerName, UTexture2D* Texture , bool bOnline);

	void UpdateChildWidget(FString PlayerName, UTexture2D* Texture, bool bOnline,int32 index);
};
