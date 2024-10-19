#include "Lobby/MatcheLobbyItemUserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/Button.h"

void UMatcheLobbyItemUserWidget::NativeConstruct()
{
	if (KickButton)
	{
		KickButton->OnClicked.AddDynamic(this, &ThisClass::KickButtonClicked);
	}
}

void UMatcheLobbyItemUserWidget::UpdatePlayerName(FString PlayerName)
{
	if (PlayerNameText)
		PlayerNameText->SetText(FText::FromString(PlayerName));
}
void UMatcheLobbyItemUserWidget::UpdateIsReady(bool IsReady)	  
{
	if (ReadyText)
		ReadyText->SetText(FText::FromString(IsReady ? "Ready" :""));
}

void UMatcheLobbyItemUserWidget::SetKickButton(bool OnButton)
{
	if(OnButton)
		KickButton->SetVisibility(ESlateVisibility::Visible);
	else
		KickButton->SetVisibility(ESlateVisibility::Hidden);
}

void UMatcheLobbyItemUserWidget::KickButtonClicked()
{
	KickButtonClickedDelegate.Execute(slotidx);
}