#include "WorldObjects/FXActor/DamageFXActor.h"
#include "Global.h"
#include "Widgets/Character/FloatingDamageWidget.h"
#include "Components/WidgetComponent.h"
#include "BaseSystem/PoolObjectActorComponent.h"


ADamageFXActor::ADamageFXActor()
{
	CHelpers::CreateComponent(this, &Mesh, "Mesh");
	CHelpers::CreateComponent<UWidgetComponent>(this, &FloatingDamageWidgetComp, "FloatingDamageWidget", Mesh);

	TSubclassOf<UFloatingDamageWidget> DamageWidget;
	CHelpers::GetClass<UFloatingDamageWidget>(&DamageWidget, "WidgetBlueprint'/Game/Developers/JJS/Widgets/WB_FloatingDamageNumbers.WB_FloatingDamageNumbers_C'");
	FloatingDamageWidgetComp->SetWidgetClass(DamageWidget);
	CHelpers::CreateActorComponent(this, &PoolObject, "PoolObject");
}

void ADamageFXActor::BeginPlay()
{
	Super::BeginPlay();
	SetDamageWidgetSizeAndLocation(FVector(0, 0, 0), FVector2D(120, 20));
	FloatingDamageWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	FloatingDamageWidget = Cast<UFloatingDamageWidget>(FloatingDamageWidgetComp->GetUserWidgetObject());
	//FloatingDamageWidget->InitWidget();
}

void ADamageFXActor::SetDamageWidgetSizeAndLocation(FVector location, FVector2D size)
{
	FloatingDamageWidgetComp->SetRelativeLocation(location);
	FloatingDamageWidgetComp->SetDrawSize(size);
}

void ADamageFXActor::SetDamageText(float damageValue)
{
	FloatingDamageWidget->SetText(damageValue);
}

void ADamageFXActor::Init()
{
	//FloatingDamageWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void ADamageFXActor::SetWidgetActive(bool bActive)
{
	if(bActive)
		FloatingDamageWidget->SetVisibility(ESlateVisibility::Collapsed);
	else
		FloatingDamageWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void ADamageFXActor::ReturnPool()
{
	OnReturnSpawner.Execute(this);
	CheckNull(PoolObject);
	PoolObject->OnReturnToPool.Execute(this);
}