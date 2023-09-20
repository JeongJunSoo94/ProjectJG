#include "Character/Components/BaseHUDComponent.h"
#include "Global.h"
#include "Widgets/UserWidget_CrossHair.h"
#include "Widgets/StatusUserWidget.h"
#include "Widgets/HealthWidget.h"
#include "Widgets/PlayerInGameWidget.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "Components/SceneComponent.h"
#include "Widgets/CharacterSkillWidget.h"

UBaseHUDComponent::UBaseHUDComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	CHelpers::GetClass<UUserWidget_CrossHair>(&CrossHairClass, "WidgetBlueprint'/Game/Developers/USER/Character/WB_CrossHair.WB_CrossHair_C'");
	CHelpers::GetClass<UPlayerInGameWidget>(&PlayerInGameClass, "WidgetBlueprint'/Game/Developers/JJS/Widgets/WB_PlayerInGameUI.WB_PlayerInGameUI_C'");
	//CHelpers::GetClass<UHealthWidget>(&HealthClass, "WidgetBlueprint'/Game/Developers/USER/Character/WB_Health.WB_Health_C'");
}

void UBaseHUDComponent::BeginPlay()
{
	Super::BeginPlay();

	//CrossHair = CreateWidget<UUserWidget_CrossHair, APlayerController>(GetController<APlayerController>(), CrossHairClass);


	//PlayerInGameUI = CreateWidget<UPlayerInGameWidget, APlayerController>(GetController<APlayerController>(), PlayerInGameClass);
	//PlayerInGameUI->AddToViewport();
	//PlayerInGameUI->SetVisibility(ESlateVisibility::Visible); 
/*	CheckNull(GetOwner());
	PlayerInGameUI->SetInit(Cast<ACBaseCharacter>(GetOwner()));*///Update(Status->GetHealth(), Status->GetMaxHealth());

	//HealthWidget->InitWidget();
	//Cast<UHealthWidget>(HealthWidget->GetUserWidgetObject())->Update(Status->GetHealth(), Status->GetMaxHealth());

}

void UBaseHUDComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UBaseHUDComponent::CreatePlayerControllerAttach(APlayerController* controller)
{
	CrossHair = CreateWidget<UUserWidget_CrossHair , APlayerController>(controller, CrossHairClass);
	PlayerInGameUI = CreateWidget<UPlayerInGameWidget, APlayerController>(controller, PlayerInGameClass);

	CrossHair->AddToViewport();
	CrossHair->SetVisibility(ESlateVisibility::Visible);

	PlayerInGameUI->AddToViewport();
	PlayerInGameUI->SetVisibility(ESlateVisibility::Visible);

	PlayerInGameUI->SetInit(this);
}

void UBaseHUDComponent::SetPlayerControllerAttach(UActorComponent* Weapon, UCActionComponent* Left, UCActionComponent* Mid, UCActionComponent* Right)
{

	PlayerInGameUI->SetInit(this);
	Clog::Log(LeftSkillClass);
	Clog::Log(Left);
	CheckNull(LeftSkillClass);
	LeftSkillClass->SetInit();
	MidSkillClass->SetInit();
	RightSkillClass->SetInit();
	Left->OnUpdateWidgetTimer.BindUObject(LeftSkillClass, &UCharacterSkillWidget::Update);
	Mid->OnUpdateWidgetTimer.BindUObject(MidSkillClass, &UCharacterSkillWidget::Update);
	Right->OnUpdateWidgetTimer.BindUObject(RightSkillClass, &UCharacterSkillWidget::Update);
}
void UBaseHUDComponent::CreateHeadHealthBar()
{
	//HealthWidget->InitWidget();
	//if (!isHiddenHeadHealthBar)
	//{
	//	CHelpers::CreateComponent<UWidgetComponent>(GetOwner(), &HealthWidget, "HealthWidget", Cast<ACharacter>(GetOwner())->GetMesh());

	//	HealthWidget->SetWidgetClass(HealthClass);
	//	HealthWidget->SetRelativeLocation(FVector(0, 0, 200));
	//	HealthWidget->SetDrawSize(FVector2D(120, 20));
	//	HealthWidget->SetWidgetSpace(EWidgetSpace::Screen);
	//}
}

void UBaseHUDComponent::HealthBarUpdate(float curHealth, float maxHealth)
{
	CheckNull(HealthClass);
	HealthClass->Update(curHealth, maxHealth);
	//Cast<UHealthWidget>(HealthWidget->GetUserWidgetObject())->Update(curHealth, maxHealth);
}