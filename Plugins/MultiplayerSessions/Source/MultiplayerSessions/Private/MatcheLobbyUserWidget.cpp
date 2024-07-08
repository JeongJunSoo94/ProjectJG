#include "MatcheLobbyUserWidget.h"
#include "MatcheLobbyItemUserWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "MultiplayerSessionsSubsystem.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

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
		}
	}

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
		if(MatcheName)
			MatcheName->SetText(FText::FromString(MultiplayerSessionsSubsystem->GetSessionLobbyName().ToString()));
	}

	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &ThisClass::StartButtonClicked);
	}
	if (LeaveButton)
	{
		LeaveButton->OnClicked.AddDynamic(this, &ThisClass::LeaveButtonClicked);
	}
	if (CharacterSelectButton)
	{
		CharacterSelectButton->OnClicked.AddDynamic(this, &ThisClass::CharacterSelectButtonClicked);
	}
	if (FriendButton)
	{
		FriendButton->OnClicked.AddDynamic(this, &ThisClass::FriendButtonClicked);
	}
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
void UMatcheLobbyUserWidget::CharacterSelectButtonClicked()
{
}
void UMatcheLobbyUserWidget::FriendButtonClicked()
{
}