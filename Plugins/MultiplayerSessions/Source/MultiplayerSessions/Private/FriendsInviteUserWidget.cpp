#include "FriendsInviteUserWidget.h"
#include "Components/TextBlock.h"
#include "Engine/Texture2D.h"
#include "Components/Image.h"
#include "Components/Button.h"

void UFriendsInviteUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Initialized();
	if (InviteButton)
	{
		InviteButton->OnClicked.AddDynamic(this, &ThisClass::InviteButtonClicked);
	}
}

void UFriendsInviteUserWidget::UpdateData(FString PlayerName, UTexture2D* Texture, bool bOnline)
{
	SetVisibility(ESlateVisibility::Visible);
	if(Texture && AvatarImage)
		AvatarImage->SetBrushFromTexture(Texture);

	if (PlayerNameText)
	{
		PlayerNameText->SetText(FText::FromString(PlayerName));
	}

	if (bOnline)
		SetRenderOpacity(1.0);
	else
		SetRenderOpacity(0.3);

	if(InviteButton)
		InviteButton->SetIsEnabled(bOnline);
}

void UFriendsInviteUserWidget::InviteButtonClicked()
{
	InviteButtonClickedDelegate.Execute(slotidx);
}

void UFriendsInviteUserWidget::Initialized()
{
	if (!PlayerNameText)
		PlayerNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("PlayerNameText")));
	if (!AvatarImage)
		AvatarImage = Cast<UImage>(GetWidgetFromName(TEXT("AvatarImage")));
	if (!InviteButton)
		InviteButton = Cast<UButton>(GetWidgetFromName(TEXT("InviteButton")));
}
