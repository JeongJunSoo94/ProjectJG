#include "Widgets/PlayerInGameWidget.h"
#include "Global.h"
#include "Widgets/CharacterSkillWidget.h"
#include "Widgets/CharacterSkillWidget.h"
#include "Widgets/HealthWidget.h"
#include "Character/BaseCharacter.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Widgets/Character/InventoryBarWidget.h"
#include "Widgets/Character/ItemInfoWidget.h"

void UPlayerInGameWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UPlayerInGameWidget::HealthUpdate(float CurHealth, float MaxHealth)
{
	if (HealthBar&& HealthText)
	{
		const float HealthPercent = CurHealth / MaxHealth;
		HealthBar->SetPercent(HealthPercent);
		HealthText->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(CurHealth), FMath::CeilToInt(MaxHealth))));
	}
}

void UPlayerInGameWidget::ShieldUpdate(float CurShield, float MaxShield)
{
	if (ShieldBar && ShieldText)
	{
		const float ShieldPercent = CurShield / MaxShield;
		ShieldBar->SetPercent(ShieldPercent);
		ShieldText->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(CurShield), FMath::CeilToInt(MaxShield))));

	}
}

void UPlayerInGameWidget::SetItemInfoCount(int32 ItemCount)
{
	if (ItemInfo&& ItemInfo->ItemCountText)
	{
		FString ItemCountText = FString::Printf(TEXT("%d"), ItemCount);
		ItemInfo->ItemCountText->SetText(FText::FromString(ItemCountText));
	}
}

void UPlayerInGameWidget::SetItemInfoCarried(int32 Carried)
{
	if (ItemInfo && ItemInfo->CarriedItemCountText)
	{
		FString CarriedText = FString::Printf(TEXT("%d"), Carried);
		ItemInfo->CarriedItemCountText->SetText(FText::FromString(CarriedText));
	}
}

void UPlayerInGameWidget::SetItemInfoType(FString ItemType)
{
	if (ItemInfo && ItemInfo->ItemTypeText)
	{
		ItemInfo->ItemTypeText->SetText(FText::FromString(ItemType));
	}
}