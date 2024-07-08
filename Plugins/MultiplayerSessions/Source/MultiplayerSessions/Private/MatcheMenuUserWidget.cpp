#include "MatcheMenuUserWidget.h"
#include "MatcheRoomUserWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/VerticalBox.h"
#include "OnlineSessionSettings.h"
#include "MenuUserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "SliderSelectorUserWidget.h"
#include "Components/EditableTextBox.h"

//void UMatcheMenuUserWidget::NativeOnInitialized()
//{
//	Super::NativeOnInitialized();
//
//}

void UMatcheMenuUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (WB_SliderSelector)
		WB_SliderSelector->SelectDelegate.AddDynamic(this, &UMatcheMenuUserWidget::OnSelectPeopleCount);
	InitMatcheItems();

	if (RoomNameTextBox)
	{
		RoomNameTextBox->OnTextChanged.AddDynamic(this, &UMatcheMenuUserWidget::OnEditableTextBoxChanged);
		RoomNameTextBox->OnTextCommitted.AddDynamic(this, &UMatcheMenuUserWidget::OnEditableTextBoxCommitted);
	}
}

void UMatcheMenuUserWidget::InitMatcheItems()
{
	TArray<UWidget*> UserWidgets = MatchesVerticalBox->GetAllChildren();
	int32 num = 0;
	for (auto value : UserWidgets)
	{
		UMatcheRoomUserWidget* temp = Cast<UMatcheRoomUserWidget>(value);
		if (temp)
		{
			MatcheRoomUserWidgets.Add(temp);
			temp->MatcheMenu = this;
			temp->SlotNum = num++;
			temp->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UMatcheMenuUserWidget::SetMatcheItems(int32 Page)
{
	TestText = "";
	int32 num = Page * MatcheRoomUserWidgets.Num() - 1;
	num = num<0 ? 0 : num;
	for (int i=0;MatcheRoomUserWidgets.Num()>i;++i)
	{
		FString OwningUserName="";
		FString NumOpenPublicConnections="";
		FString NumPublicConnections="";
		FString PingInMs="";
		if (MenuUser->GetSearchResultsInfo(num, OwningUserName, NumOpenPublicConnections, NumPublicConnections, PingInMs))
		{
			NumOpenPublicConnections.Append("/");
			NumOpenPublicConnections.Append(NumPublicConnections);
			MatcheRoomUserWidgets[i]->UpdateProperty(OwningUserName,NumOpenPublicConnections, PingInMs);
			MatcheRoomUserWidgets[i]->SlotNum = num++;
			MatcheRoomUserWidgets[i]->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			MatcheRoomUserWidgets[i]->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	//MenuUser->TestTextBox->SetText(FText::FromString(TestText));
}

void UMatcheMenuUserWidget::OnSelectMatcheItem(int32 SelectSlotNum)
{
	SelectRoomNum = SelectSlotNum;
}

void UMatcheMenuUserWidget::OnSelectPeopleCount(FString PeopleCount)
{
	SessionNumPublicConnections = FCString::Atoi(*PeopleCount);
}

void UMatcheMenuUserWidget::OnEditableTextBoxChanged(const FText& Text)
{
	SessionLobbyName = *Text.ToString();
}

void UMatcheMenuUserWidget::OnEditableTextBoxCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	SessionLobbyName = *Text.ToString();
}