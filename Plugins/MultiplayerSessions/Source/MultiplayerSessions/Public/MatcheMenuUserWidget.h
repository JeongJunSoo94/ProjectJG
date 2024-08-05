// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MatcheMenuUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UMatcheMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()
//public:
//	virtual void NativeOnInitialized() override;
protected:
	virtual void NativeConstruct() override;
public:

	UPROPERTY(meta = (BindWidget))
		class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
		UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
		UButton* Refresh;

	FString TestText;

	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* RoomNameTextBox;

	class UMenuUserWidget* MenuUser;

	UPROPERTY(meta = (BindWidget))
		class USliderSelectorUserWidget* WB_SliderSelector;

	UPROPERTY(meta = (BindWidget))
		class UVerticalBox* MatchesVerticalBox;

	FString SessionLobbyName{ TEXT("") };

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32 SessionNumPublicConnections {
		2
	};

	TArray<class UMatcheRoomUserWidget*> MatcheRoomUserWidgets;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32 SelectRoomNum {-1};

	UFUNCTION()
		void InitMatcheItems();

	UFUNCTION()
		void SetMatcheItems(int32 Page);

	UFUNCTION()
		void OnSelectMatcheItem(int32 SelectSlotNum);

	UFUNCTION()
		void OnSelectPeopleCount(FString PeopleCount);

	UFUNCTION()
		void OnEditableTextBoxChanged(const FText& Text);

	UFUNCTION()
		void OnEditableTextBoxCommitted(const FText& Text, ETextCommit::Type CommitMethod);
};
