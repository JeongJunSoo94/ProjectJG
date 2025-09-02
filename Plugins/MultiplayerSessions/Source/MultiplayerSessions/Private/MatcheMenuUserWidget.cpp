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

	if (WB_RoomSliderSelector)
		WB_RoomSliderSelector->SelectDelegate.AddDynamic(this, &UMatcheMenuUserWidget::OnSelectRoomPageCount);

	if (LogButton)
	{
		LogButton->OnClicked.AddDynamic(this, &ThisClass::LogClicked);
	}

	if (RoomNameTextBox)
	{
		RoomNameTextBox->OnTextChanged.AddDynamic(this, &UMatcheMenuUserWidget::OnEditableTextBoxChanged);
		RoomNameTextBox->OnTextCommitted.AddDynamic(this, &UMatcheMenuUserWidget::OnEditableTextBoxCommitted);
	}
}

void UMatcheMenuUserWidget::LogClicked()
{
	//현재 접속중인 세션 정보 확인
	//FString OwningUserName = "";
	//int32 NumOpenPublicConnections;
	//int32 NumPublicConnections;
	//FString Players = "";
	//FString PingInMs = "";
	//GEngine->AddOnScreenDebugMessage(3, 200.0f, FColor::Purple, MenuUser);

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
		int32 NumOpenPublicConnections;
		int32 NumPublicConnections;
		FString Players = "";
		FString PingInMs="";
		if (MenuUser->GetSearchResultsInfo(num, OwningUserName, NumOpenPublicConnections, NumPublicConnections, PingInMs))
		{
			Players.AppendInt(NumPublicConnections - NumOpenPublicConnections);
			Players.Append("/");
			Players.AppendInt(NumPublicConnections);
			MatcheRoomUserWidgets[i]->UpdateProperty(OwningUserName, Players, PingInMs);
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
	//누르면 세션 정보 나오게 만들기


	if (SelectRoomNum >= 0)
	{
		//클릭하면 이전 룸의 컬러를 변경한다.
		MatcheRoomUserWidgets[SelectRoomNum]->UpdateSelectButton(false);
	}
	//선택된 룸의 컬러를 변경한다.
	SelectRoomNum = SelectSlotNum;
	MatcheRoomUserWidgets[SelectRoomNum]->UpdateSelectButton(true);
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

void UMatcheMenuUserWidget::SetSliderOption(int32 Page)
{
	if (WB_RoomSliderSelector->Options.Num() == Page+1)
		return;
	WB_RoomSliderSelector->Options.Empty();
	for (int i = 0; i <= Page; ++i)
	{
		FString IntAsString = FString::FromInt(i+1);
		WB_RoomSliderSelector->Options.Add(FName(*IntAsString));
	}
	WB_RoomSliderSelector->CurIdx = 0;
	WB_RoomSliderSelector->UpdateText();
}

void UMatcheMenuUserWidget::OnSelectRoomPageCount(FString RoomPageCount)
{
	SetMatcheItems(FCString::Atoi(*RoomPageCount));
}