#include "Lobby/MatcheLobbyUserWidget.h"
#include "Lobby/MatcheLobbyItemUserWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "MultiplayerSessionsSubsystem.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Lobby/MatchePlayerController.h"
#include "Interfaces/OnlinePresenceInterface.h"
#include "FriendsInviteUserWidget.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Lobby/MatcheLobbyGameState.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameSession.h"
#include "GameFramework/PlayerState.h"

void UMatcheLobbyUserWidget::NativeConstruct()
{
	TArray<UWidget*> UserWidgets = MatchesVerticalBox->GetAllChildren();
	int32 num = 0;
	for (auto value : UserWidgets)
	{
		UMatcheLobbyItemUserWidget* temp = Cast<UMatcheLobbyItemUserWidget>(value);
		if (temp)
		{
			MatcheLobbyItemUserWidgets.Add(temp);

			temp->SetVisibility(ESlateVisibility::Hidden);
			temp->KickButtonClickedDelegate.BindUFunction(this, FName("OnKickButtonClicked"));
			temp->slotidx = num++;
		}
	}

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
		if(MatcheName)
			MatcheName->SetText(FText::FromString(MultiplayerSessionsSubsystem->GetSessionLobbyName()));
	}

	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &ThisClass::StartButtonClicked);
	}
	if (LeaveButton)
	{
		LeaveButton->OnClicked.AddDynamic(this, &ThisClass::LeaveButtonClicked);
	}
	//if (CharacterSelectButton)
	//{
	//	CharacterSelectButton->OnClicked.AddDynamic(this, &ThisClass::CharacterSelectButtonClicked);
	//}
	if (FriendButton)
	{
		FriendButton->OnClicked.AddDynamic(this, &ThisClass::FriendButtonClicked);
	}

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			MatchePlayerController = Cast<AMatchePlayerController>(PlayerController);
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}
	MatcheLobbyGameState = GetWorld()->GetGameState<AMatcheLobbyGameState>();
}

void UMatcheLobbyUserWidget::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	MenuTearDown();
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}
UMatcheLobbyItemUserWidget* UMatcheLobbyUserWidget::GetMatchePalyerWidget(int32 idx)
{
	if(MatcheLobbyItemUserWidgets.IsValidIndex(idx))
	{
		return MatcheLobbyItemUserWidgets[idx];
	}
	return nullptr;
}

void UMatcheLobbyUserWidget::StartButtonClicked()
{
	if (MatchePlayerController)
	{
		if (!MatchePlayerController->HasAuthority())
		{
			MatchePlayerController->bIsReady = MatchePlayerController->bIsReady ? false: true ;
			MatchePlayerController->ClickStartButton(MatchePlayerController->bIsReady);
		}
		else
		{
			if (MatcheLobbyGameState->PlayerArray.Num() == MultiplayerSessionsSubsystem->GetSessionPlayerNum())
			{
				MatchePlayerController->bIsReady = MatchePlayerController->bIsReady ? false : true;
				MatchePlayerController->ClickStartButton(MatchePlayerController->bIsReady);
			}
		}
	}
}
void UMatcheLobbyUserWidget::LeaveButtonClicked()
{
	UWorld* World = GetWorld();
	if (World)
	{
		UGameplayStatics::OpenLevel(World, *PathToMainMenu, true);
		MultiplayerSessionsSubsystem->DestroySession();
	}
}
//void UMatcheLobbyUserWidget::CharacterSelectButtonClicked()
//{
//}
void UMatcheLobbyUserWidget::FriendButtonClicked()
{
	if(FriendListUserWidget->GetVisibility()== ESlateVisibility::Visible)
		FriendListUserWidget->SetVisibility(ESlateVisibility::Hidden);
	else
		FriendListUserWidget->SetVisibility(ESlateVisibility::Visible);
	//리스트 초기화
	MultiplayerSessionsSubsystem->FetchFriendsList();

	FriendListUserWidget->ClearFriendList();

	int32 i = 0;
	for (const TSharedRef<FOnlineFriend>& Friend : MultiplayerSessionsSubsystem->GetFriendsList())
	{
		const FOnlineUserPresence& FriendPresence = Friend->GetPresence();
		EOnlinePresenceState::Type FriendOnlineState = FriendPresence.Status.State;
		
		bool bIsOnline;
		switch (FriendOnlineState)
		{
			case EOnlinePresenceState::Type::Online:
				bIsOnline = true;
				break;
			default:
				bIsOnline = false;
				break;
		}
		TSharedPtr<const FUniqueNetId> FriendNet= Friend->GetUserId();
		FString temp = Friend->GetUserId()->ToString();
		UTexture2D* texture2d = MultiplayerSessionsSubsystem->GetSteamFriendAvatar(FCString::Strtoui64(*temp,nullptr,10));

		if (FriendListUserWidget->FriendContainer->GetChildrenCount() > i)
		{
			FriendListUserWidget->UpdateChildWidget(Friend->GetDisplayName(), texture2d, bIsOnline, i);
		}
		else
		{
			UFriendsInviteUserWidget* FriendsInviteUserWidget = Cast<UFriendsInviteUserWidget>(FriendListUserWidget->AddChildWidget(Friend->GetDisplayName(), texture2d,bIsOnline));
			if (FriendsInviteUserWidget)
			{
				FriendsInviteUserWidget->InviteButtonClickedDelegate.BindUFunction(this, FName("OnInviteButtonClicked"));
			}
		}
		++i;
	}
}

void UMatcheLobbyUserWidget::OnInviteButtonClicked(int32 SlotIdx)
{
	UWorld* World = GetWorld();
	if (World)
	{
		const ULocalPlayer* LocalPlayer = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetLocalPlayer();
		MultiplayerSessionsSubsystem->SendSessionInviteToFriend(FCString::Strtoi(*LocalPlayer->GetPreferredUniqueNetId().ToString(), nullptr, 10), *MultiplayerSessionsSubsystem->GetFriendsList()[SlotIdx]->GetUserId());
	}
}

void UMatcheLobbyUserWidget::OnKickButtonClicked(int32 SlotIdx)
{
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerState* PlayerState = MatcheLobbyGameState->GetFindPlayerState(SlotIdx);
		if (PlayerState)
		{
			APlayerController* PlayerController = PlayerState->GetOwner<APlayerController>();
			if (PlayerController)
			{
				AGameModeBase* GameMode = World->GetAuthGameMode<AGameModeBase>();
				if (GameMode && GameMode->GameSession)
				{
					FText KickReason = FText::FromString(TEXT("You have been kicked from the session"));

					if (GameMode->GameSession->KickPlayer(PlayerController, KickReason))
					{
						UE_LOG(LogTemp, Log, TEXT("Player kicked successfully"));
					}
					else
					{
						UE_LOG(LogTemp, Log, TEXT("Failed to kick player"));
					}
				}
			}
		}
	}
}

void UMatcheLobbyUserWidget::RemoveBindings()
{
	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->MultiplayerOnFindSessionsComplete.RemoveAll(this);
		MultiplayerSessionsSubsystem->MultiplayerOnFindSessionsComplete.RemoveAll(this);
	}
}

void UMatcheLobbyUserWidget::NativeDestruct()
{
	RemoveBindings();
	Super::NativeDestruct();
}


void UMatcheLobbyUserWidget::MenuTearDown()
{
	RemoveFromParent();
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false);
		}
	}
}