#include "BaseSystem/GameHUD.h"
#include "Global.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/HealthWidget.h"
#include "Widgets/UserWidget_CrossHair.h"
#include "Widgets/PlayerInGameWidget.h"
#include "Widgets/StatusUserWidget.h"
#include "Components/WidgetComponent.h"
#include "Character/Components/CActionComponent.h"

AGameHUD::AGameHUD()
{
	CHelpers::GetClass<UPlayerInGameWidget>(&PlayerInGameClass, "WidgetBlueprint'/Game/Developers/JJS/Widgets/WB_PlayerInGameUI.WB_PlayerInGameUI_C'");
	CHelpers::GetClass<UUserWidget_CrossHair>(&CrossHairClass, "WidgetBlueprint'/Game/Developers/USER/Character/WB_CrossHair.WB_CrossHair_C'");

}

void AGameHUD::BeginPlay()
{
	Super::BeginPlay();
	if (PlayerInGameClass)
	{
		PlayerInGameUI = CreateWidget<UPlayerInGameWidget>(GetWorld(), PlayerInGameClass);
		if (PlayerInGameUI)
		{
			PlayerInGameUI->AddToViewport();
			PlayerInGameUI->SetVisibility(ESlateVisibility::Visible);
			//PlayerInGameUI->SetInit(this);
		}
	}

	if (CrossHairClass)
	{
		CrossHair = CreateWidget<UUserWidget_CrossHair>(GetWorld(), CrossHairClass);
		if (CrossHair)
		{
			CrossHair->AddToViewport();
			CrossHair->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void AGameHUD::SetHUDPlayerControllerSkillBind(UActorComponent* Weapon, UCActionComponent* Left, UCActionComponent* Mid, UCActionComponent* Right)
{
	PlayerInGameUI->GetWB_CharacterSkill_Left()->SetInit();
	PlayerInGameUI->GetWB_CharacterSkill_Mid()->SetInit();
	PlayerInGameUI->GetWB_CharacterSkill_Right()->SetInit();
	Left->OnUpdateWidgetTimer.BindUObject(PlayerInGameUI->GetWB_CharacterSkill_Left() , &UCharacterSkillWidget::Update);
	Mid->OnUpdateWidgetTimer.BindUObject(PlayerInGameUI->GetWB_CharacterSkill_Mid(), &UCharacterSkillWidget::Update);
	Right->OnUpdateWidgetTimer.BindUObject(PlayerInGameUI->GetWB_CharacterSkill_Right(), &UCharacterSkillWidget::Update);
}

void AGameHUD::HealthBarUpdate(float curHealth, float maxHealth)
{
	CheckNull(PlayerInGameUI);
	PlayerInGameUI->GetWB_Health()->Update(curHealth, maxHealth);
}