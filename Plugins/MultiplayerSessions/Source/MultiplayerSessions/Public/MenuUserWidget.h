#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MenuUserWidget.generated.h"

UCLASS()
class MULTIPLAYERSESSIONS_API UMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
		void MenuSetup(int32 NumberOfPublicConnections = 4, FString TypeOfMatch = FString(TEXT("FreeForAll")), FString LobbyPath = FString(TEXT("/Game/Developers/JJS/TestMap/MultiLobby/MultiLobby")));

	bool GetSearchResultsInfo(int32 Index, FString& OwningUserName, FString& NumOpenPublicConnections, FString& NumPublicConnections, FString& PingInMs);
	//
	//UPROPERTY(meta = (BindWidget))
	//	class UTextBlock* TestTextBox;

	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* WidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
		class UButton* UndoButton;

	UFUNCTION()
		void RefreshButtonClicked();
protected:

	virtual bool Initialize() override;
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

	UFUNCTION()
		void OnCreateSession(bool bWasSuccessful);
	void OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);
	void OnJoinSession(EOnJoinSessionCompleteResult::Type Result);
	UFUNCTION()
		void OnDestroySession(bool bWasSuccessful);
	UFUNCTION()
		void OnStartSession(bool bWasSuccessful);

private:

	UPROPERTY(meta = (BindWidget))
		class UHomeMenuUserWidget* WB_Home;

	UPROPERTY(meta = (BindWidget))
		class UMatcheMenuUserWidget* WB_Matche;


	UFUNCTION()
		void HostButtonClicked();

	UFUNCTION()
		void JoinButtonClicked();

	UFUNCTION()
		void UndoButtonClicked();

	void MenuTearDown();

	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString MatchType {
		TEXT("FreeForAll")
	};

	FString PathToLobby{ TEXT("") };

	 APlayerController*  PlayerController;
};
