#include "FriendListUserWidget.h"
#include "Components/VerticalBox.h"

void UFriendListUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UFriendListUserWidget::ClearFriendList()
{
	//FriendContainer->ClearChildren();
	for (auto FriendWidget : FriendContainer->GetAllChildren())
	{
		FriendWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

UFriendsInviteUserWidget* UFriendListUserWidget::AddChildWidget(FString PlayerName, UTexture2D* Texture, bool bOnline)
{
	if (FriendWidgetClass)
	{
		FriendContainer->AddChild(CreateWidget<UFriendsInviteUserWidget>(GetWorld(), FriendWidgetClass));
		UFriendsInviteUserWidget* FriendWidget = Cast<UFriendsInviteUserWidget>(FriendContainer->GetChildAt(FriendContainer->GetChildrenCount()-1));
		FriendWidget->UpdateData(PlayerName, Texture,  bOnline);
		FriendWidget->slotidx = FriendContainer->GetChildrenCount()-1;
		return FriendWidget;
	}

	return nullptr;
}

void UFriendListUserWidget::UpdateChildWidget(FString PlayerName, UTexture2D* Texture, bool bOnline, int32 index)
{
	UFriendsInviteUserWidget* FriendWidget = Cast<UFriendsInviteUserWidget>(FriendContainer->GetChildAt(index));
	FriendWidget->UpdateData(PlayerName, Texture, bOnline);
}
