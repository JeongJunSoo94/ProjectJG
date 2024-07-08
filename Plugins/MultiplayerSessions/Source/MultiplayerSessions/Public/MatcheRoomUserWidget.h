// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MatcheRoomUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UMatcheRoomUserWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
public:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ServerNameText;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* PlayersText;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* PingText;
	UPROPERTY(meta = (BindWidget))
		class UButton* MatcheSelectButton;

	int32 SlotNum=-1;
	UFUNCTION()
		void SelectButtonClicked();

	UPROPERTY(EditAnywhere,BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UMatcheMenuUserWidget* MatcheMenu;

	void UpdateProperty(FString RoomName,FString PlayerNum,FString Ping);

};
