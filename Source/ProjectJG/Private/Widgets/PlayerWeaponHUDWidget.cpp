#include "Widgets/PlayerWeaponHUDWidget.h"
#include "Components/TextBlock.h"

void UPlayerWeaponHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UPlayerWeaponHUDWidget::SetInit(UTexture2D* image)
{
	//BackGroundImage->
}

void UPlayerWeaponHUDWidget::Update(float curValue, float maxValue)
{
	//TextBlock->SetText(TEXT("%d / %d", curValue, maxValue));
}