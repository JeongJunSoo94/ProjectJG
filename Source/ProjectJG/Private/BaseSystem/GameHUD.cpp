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
#include "Widgets/Pause/InGameMenuUserWidget.h"
#include "Widgets/Pause/SoundControlWidget.h"
#include "Widgets/Character/QuestWidget.h"

AGameHUD::AGameHUD()
{
	CHelpers::GetClass<UPlayerInGameWidget>(&PlayerInGameClass, "WidgetBlueprint'/Game/Developers/JJS/Widgets/WB_PlayerInGameUI.WB_PlayerInGameUI_C'");
	//CHelpers::GetClass<UUserWidget_CrossHair>(&CrossHairClass, "WidgetBlueprint'/Game/Developers/USER/Character/WB_CrossHair.WB_CrossHair_C'");
	CHelpers::GetClass<UInGameMenuUserWidget>(&InGameMenuClass, "WidgetBlueprint'/Game/Developers/JJS/Widgets/WB_InGameMenu.WB_InGameMenu_C'");
	CHelpers::GetClass<USoundControlWidget>(&SoundControlClass, "WidgetBlueprint'/Game/Developers/GohyeongJu/Characters/Level/Wigets/WB_SoundWidget.WB_SoundWidget_C'");
	CHelpers::GetClass<UQuestWidget>(&QuestWidgetClass, "WidgetBlueprint'/Game/Developers/GohyeongJu/Characters/Level/Wigets/WB_Quest_window.WB_Quest_window_C'");

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
			PlayerInGameUI->SetVisibility(ESlateVisibility::HitTestInvisible);
			//PlayerInGameUI->SetInit(this);
		}
	}

	//if (CrossHairClass)
	//{
	//	CrossHair = CreateWidget<UUserWidget_CrossHair>(PlayerInGameUI, CrossHairClass);
	//	if (CrossHair)
	//	{
	//		CrossHair->AddToViewport();
	//		CrossHair->SetVisibility(ESlateVisibility::Visible);
	//	}
	//}

	if (InGameMenuClass)
	{
		InGameMenuUI = CreateWidget<UInGameMenuUserWidget>(PlayerInGameUI, InGameMenuClass);
		if (InGameMenuUI)
		{
			InGameMenuUI->AddToViewport();
			InGameMenuUI->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (SoundControlClass)
	{
		SoundControlUI = CreateWidget<USoundControlWidget>(PlayerInGameUI, SoundControlClass);
		if (SoundControlUI)
		{
			SoundControlUI->AddToViewport();
			SoundControlUI->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	//if (QuestWidgetClass)
	//{
	//	QuestWidgetUI = CreateWidget<UQuestWidget>(PlayerInGameUI, QuestWidgetClass);
	//	if (QuestWidgetUI)
	//	{
	//		QuestWidgetUI->AddToViewport();
	//		QuestWidgetUI->SetVisibility(ESlateVisibility::HitTestInvisible);
	//	}
	//}
}

void AGameHUD::SetHUDPlayerControllerSkillBind(UActorComponent* Weapon, UCActionComponent* Left, UCActionComponent* Mid, UCActionComponent* Right)
{
	//PlayerInGameUI->GetWB_CharacterSkill_Left()->SetInit();
	//PlayerInGameUI->GetWB_CharacterSkill_Mid()->SetInit();
	//PlayerInGameUI->GetWB_CharacterSkill_Right()->SetInit();
	//Left->OnUpdateWidgetTimer.BindUObject(PlayerInGameUI->GetWB_CharacterSkill_Left() , &UCharacterSkillWidget::Update);
	//Mid->OnUpdateWidgetTimer.BindUObject(PlayerInGameUI->GetWB_CharacterSkill_Mid(), &UCharacterSkillWidget::Update);
	//Right->OnUpdateWidgetTimer.BindUObject(PlayerInGameUI->GetWB_CharacterSkill_Right(), &UCharacterSkillWidget::Update);
}

void AGameHUD::HealthBarUpdate(float curHealth, float maxHealth)
{
	CheckNull(PlayerInGameUI);
	PlayerInGameUI->HealthUpdate(curHealth, maxHealth);
}

void AGameHUD::IsMenuBarActive(bool isActive)
{
	bPause = bPause ? false : true;
	if (isActive)
		InGameMenuUI->SetVisibility(ESlateVisibility::Visible);
	else
		InGameMenuUI->SetVisibility(ESlateVisibility::Collapsed);

	UGameplayStatics::SetGamePaused(GetWorld(), isActive);
	UGameplayStatics::GetPlayerController(this, 0)->SetShowMouseCursor(isActive);
}

void AGameHUD::IsSoundActive(bool isActive)
{
	if (isActive)
	{
		SoundControlUI->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SoundControlUI->SetVisibility(ESlateVisibility::Collapsed);
	}

}

void AGameHUD::DrawHUD()
{
	Super::DrawHUD();

	FVector2D ViewportSize;
	if (GEngine)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		const FVector2D ViewportCenter(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);

		float SpreadScaled = CrosshairSpreadMax * HUDPackage.CrosshairSpread;
		
		if (HUDPackage.CrosshairsCenter)
		{
			
			FVector2D Spread(0.f, 0.f);
			DrawCrosshair(HUDPackage.CrosshairsCenter, ViewportCenter, Spread, HUDPackage.CrosshairsColor);
		}
		if (HUDPackage.CrosshairsLeft)
		{
			FVector2D Spread(-SpreadScaled, 0.f);
			DrawCrosshair(HUDPackage.CrosshairsLeft, ViewportCenter, Spread, HUDPackage.CrosshairsColor);
		}
		if (HUDPackage.CrosshairsRight)
		{
			FVector2D Spread(SpreadScaled, 0.f);
			DrawCrosshair(HUDPackage.CrosshairsRight, ViewportCenter, Spread, HUDPackage.CrosshairsColor);
		}
		if (HUDPackage.CrosshairsTop)
		{
			FVector2D Spread(0.f, -SpreadScaled);
			DrawCrosshair(HUDPackage.CrosshairsTop, ViewportCenter, Spread, HUDPackage.CrosshairsColor);
		}
		if (HUDPackage.CrosshairsBottom)
		{
			FVector2D Spread(0.f, SpreadScaled);
			DrawCrosshair(HUDPackage.CrosshairsBottom, ViewportCenter, Spread, HUDPackage.CrosshairsColor);
		}
	}
}

void AGameHUD::DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor CrosshairColor)
{
	const float TextureWidth = Texture->GetSizeX();
	const float TextureHeight = Texture->GetSizeY();
	const FVector2D TextureDrawPoint(
		ViewportCenter.X - (TextureWidth / 2.f) + Spread.X,
		ViewportCenter.Y - (TextureHeight / 2.f) + Spread.Y
	);

	DrawTexture(
		Texture,
		TextureDrawPoint.X,
		TextureDrawPoint.Y,
		TextureWidth,
		TextureHeight,
		0.f,
		0.f,
		1.f,
		1.f,
		CrosshairColor
	);
}