#include "Lobby/MatcheLobbyItemUserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/Button.h"

void UMatcheLobbyItemUserWidget::NativeConstruct()
{

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
void UMatcheLobbyItemUserWidget::UpdatePing(FString Ping)			  
{
	if (PingText)
		PingText->SetText(FText::FromString(Ping));
}
void UMatcheLobbyItemUserWidget::SetKickButton(bool OnButton)
{
	if(OnButton)
		MatcheSelectButton->SetVisibility(ESlateVisibility::Visible);
	else
		MatcheSelectButton->SetVisibility(ESlateVisibility::Hidden);
}