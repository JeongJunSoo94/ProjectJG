#include "Widgets/PlayerInGameWidget.h"
#include "Global.h"
#include "Widgets/CharacterSkillWidget.h"
#include "Character/Components/BaseHUDComponent.h"
#include "Widgets/CharacterSkillWidget.h"
#include "Widgets/HealthWidget.h"

void ::UPlayerInGameWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UPlayerInGameWidget::SetBaseWidget(UBaseHUDComponent* HUD,UHealthWidget* HealthWidget , UCharacterSkillWidget* abilityLeft, UCharacterSkillWidget* abilityMid, UCharacterSkillWidget* abilityRight)
{
	CheckNull(HUD);

	HUD->SetHealthWidget(HealthWidget);
	HUD->SetSkillWidget(abilityLeft, abilityMid, abilityRight);
	//player->SetHealthWidget(healthWidget);
}

void UPlayerInGameWidget::Update(float curTime, float maxTime)
{

}
