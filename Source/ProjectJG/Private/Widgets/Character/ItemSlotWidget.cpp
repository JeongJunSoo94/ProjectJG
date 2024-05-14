#include "Widgets/Character/ItemSlotWidget.h"
#include "Global.h"
#include "WorldObjects/Item/Item.h"
#include "Components/TextBlock.h"
#include "Engine/Texture2D.h"
#include "Styling/SlateBrush.h"
#include "Components/Image.h"

void UItemSlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}


void UItemSlotWidget::UpdateItemSlotImage(UTexture2D* backgroundImage, UTexture2D* itemIcon, UTexture2D* ammoIcon)
{
	if (BackgroundImage)
	{
		BackgroundImage->SetVisibility(ESlateVisibility::Visible);
		BackgroundImage->SetBrushFromTexture(backgroundImage,true);

	}
	if (ItemIcon)
	{
		ItemIcon->SetVisibility(ESlateVisibility::Visible);
		ItemIcon->SetBrushFromTexture(itemIcon, true);
	}
	if (AmmoIcon)
	{
		AmmoIcon->SetVisibility(ESlateVisibility::Visible);
		AmmoIcon->SetBrushFromTexture(ammoIcon, true);
	}
}

void UItemSlotWidget::SelectItemSlotImage(bool value)
{ 
}

void UItemSlotWidget::UpdateItemSlotCount(FString Count)
{
	if(ItemCountText)
		ItemCountText->SetText(FText::FromString(Count));
}