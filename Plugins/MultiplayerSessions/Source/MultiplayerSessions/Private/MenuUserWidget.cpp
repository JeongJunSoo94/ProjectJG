#include "MenuUserWidget.h"
#include "Components/Button.h"
#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Components/TextBlock.h"
#include "MatcheRoomUserWidget.h"
#include "Components/WidgetComponent.h"
#include "MatcheMenuUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Engine.h"
#include "HomeMenuUserWidget.h"
#include "Components/WidgetSwitcher.h"

void UMenuUserWidget::MenuSetup(int32 NumberOfPublicConnections, FString TypeOfMatch, FString LobbyPath)
{
	PathToLobby = FString::Printf(TEXT("%s?listen"), *LobbyPath);
	//SessionNumPublicConnections = NumberOfPublicConnections;
	MatchType = TypeOfMatch;
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	UWorld* World = GetWorld();
	if (World)
	{
		PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}

	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->MultiplayerOnCreateSessionComplete.AddDynamic(this, &ThisClass::OnCreateSession);
		MultiplayerSessionsSubsystem->MultiplayerOnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSessions);
		MultiplayerSessionsSubsystem->MultiplayerOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSession);
		MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &ThisClass::OnDestroySession);
		MultiplayerSessionsSubsystem->MultiplayerOnStartSessionComplete.AddDynamic(this, &ThisClass::OnStartSession);
	}

	if (WB_Matche)
		WB_Matche->MenuUser = this;
	if (WB_Home)
		WB_Home->MenuUser = this;
	if (WidgetSwitcher->GetActiveWidgetIndex() == 0)
	{
		UndoButton->SetVisibility(ESlateVisibility::Hidden);
	}
}

bool UMenuUserWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (WB_Matche->HostButton)
	{
		WB_Matche->HostButton->OnClicked.AddDynamic(this, &ThisClass::HostButtonClicked);
	}
	if (WB_Matche->JoinButton)
	{
		WB_Matche->JoinButton->OnClicked.AddDynamic(this, &ThisClass::JoinButtonClicked);
	}
	if (WB_Matche->Refresh)
	{
		WB_Matche->Refresh->OnClicked.AddDynamic(this, &ThisClass::RefreshButtonClicked);
	}
	if (UndoButton)
	{
		UndoButton->OnClicked.AddDynamic(this, &ThisClass::UndoButtonClicked);
	}
	return true;
}

void UMenuUserWidget::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	MenuTearDown();
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}

void UMenuUserWidget::OnCreateSession(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UWorld * World = GetWorld();
		
		UGameplayStatics::OpenLevel(World, *PathToLobby,true);
		//PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
		//if (PlayerController)
		//{
		//	PlayerController->ClientTravel(PathToLobby, ETravelType::TRAVEL_Absolute);
		//}
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Green,
				FString(TEXT("Success to create session!"))
			);
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Red,
				FString(TEXT("Failed to create session!"))
			);
		}
		WB_Matche->HostButton->SetIsEnabled(true);
	}
}

void UMenuUserWidget::OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful)
{
	if (MultiplayerSessionsSubsystem == nullptr)
	{
		return;
	}
	WB_Matche->SetMatcheItems(0);
	WB_Matche->Refresh->SetIsEnabled(true);
}

void UMenuUserWidget::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			FString Address;
			SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);

			PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
			if (PlayerController)
			{
				PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
				//TestTextBox->SetText(FText::FromString("OnJoinSession"));
			}
		}
	}
}

void UMenuUserWidget::OnDestroySession(bool bWasSuccessful)
{
}

void UMenuUserWidget::OnStartSession(bool bWasSuccessful)
{
}

void UMenuUserWidget::RefreshButtonClicked()
{
	WB_Matche->Refresh->SetIsEnabled(false);
	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->FindSessions(100);
		//MultiplayerSessionsSubsystem->CreateSession(SessionNumPublicConnections, MatchType);
	}
}

void UMenuUserWidget::HostButtonClicked()
{
	WB_Matche->HostButton->SetIsEnabled(false);
	if (MultiplayerSessionsSubsystem)
	{
		FString SessionName;
		if (GEngine)
		{
			if(WB_Matche->SessionLobbyName =="")
			{
				SessionName = PlayerController->GetPlayerState<APlayerState>()->GetPlayerName();
				SessionName.Append("'s Room");
			}
			else
			{
				SessionName = *WB_Matche->SessionLobbyName;
				GEngine->AddOnScreenDebugMessage(
					-2,
					15.f,
					FColor::Green,
					FString::Printf(TEXT("%s"), *WB_Matche->SessionLobbyName)
				);
			}
		}
		MultiplayerSessionsSubsystem->CreateSession(FName(SessionName), WB_Matche->SessionNumPublicConnections, MatchType);
	}
}

void UMenuUserWidget::JoinButtonClicked()
{
	WB_Matche->JoinButton->SetIsEnabled(false);
	if (MultiplayerSessionsSubsystem)
	{
		if (WB_Matche->SelectRoomNum >= 0 && MultiplayerSessionsSubsystem->GetSearchResults().Num() > WB_Matche->SelectRoomNum)
		{
			MultiplayerSessionsSubsystem->JoinSession(MultiplayerSessionsSubsystem->GetSearchResults()[WB_Matche->SelectRoomNum]);
			//TestTextBox->SetText(FText::FromString("JoinButtonClicked"));
		}
		//MultiplayerSessionsSubsystem->FindSessions(10000);
	}
}


void UMenuUserWidget::UndoButtonClicked()
{
	WidgetSwitcher->SetActiveWidgetIndex(0);
	UndoButton->SetVisibility(ESlateVisibility::Hidden);
}

void UMenuUserWidget::MenuTearDown()
{
	RemoveFromParent();
	UWorld* World = GetWorld();
	if (World)
	{
		PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false);
		}
	}
}

bool UMenuUserWidget::GetSearchResultsInfo(int32 Index, FString& OwningUserName, FString& NumOpenPublicConnections, FString& NumPublicConnections, FString& PingInMs)
{
	if (MultiplayerSessionsSubsystem->GetSearchResults().Num() > Index)
	{
		MultiplayerSessionsSubsystem->GetSearchResults()[Index].Session.SessionSettings.Get(FName("SESSION_LOBBY_NAME"), OwningUserName);
		if(OwningUserName=="")
			OwningUserName = MultiplayerSessionsSubsystem->GetSearchResults()[Index].Session.OwningUserName;

		NumOpenPublicConnections = FString::FromInt(MultiplayerSessionsSubsystem->GetSearchResults()[Index].Session.NumOpenPublicConnections);
		NumPublicConnections = FString::FromInt(MultiplayerSessionsSubsystem->GetSearchResults()[Index].Session.SessionSettings.NumPublicConnections);
		PingInMs = FString::FromInt(MultiplayerSessionsSubsystem->GetSearchResults()[Index].PingInMs);
		return true;
	}
	return false;
}