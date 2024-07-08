#include "MatcheRoomUserWidget.h"
#include "MatcheMenuUserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/Button.h"

void UMatcheRoomUserWidget::SelectButtonClicked()
{
	if(MatcheMenu)
		MatcheMenu->OnSelectMatcheItem(SlotNum);
}

void UMatcheRoomUserWidget::UpdateProperty(FString RoomName, FString PlayerNum, FString Ping)
{
	if (ServerNameText)
		ServerNameText->SetText(FText::FromString(RoomName));
	if (PlayersText)
		PlayersText->SetText(FText::FromString(PlayerNum));
	if (PingText)
		PingText->SetText(FText::FromString(Ping));
}

void UMatcheRoomUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (MatcheSelectButton)
	{
		MatcheSelectButton->OnClicked.AddDynamic(this, &ThisClass::SelectButtonClicked);
	}
}