#include "Character/Components/StatusComponent.h"
#include "Global.h"
//#include "Net/UnrealNetwork.h"


UStatusComponent::UStatusComponent()
{
	//CHelpers::CreateComponent<UWidgetComponent>(this, &HealthWidget, "HealthWidget", GetMesh());

	//TSubclassOf<UHealthWidget> healthClass;
	//CHelpers::GetClass<UHealthWidget>(&healthClass, "WidgetBlueprint'/Game/Developers/USER/Character/WB_Health.WB_Health_C'");
	//HealthWidget->SetWidgetClass(healthClass);
	//SetHealthWidgetSizeAndLocation(FVector(0, 0, 200), FVector2D(120, 20));
	//HealthWidget->SetWidgetSpace(EWidgetSpace::Screen);
}

void UStatusComponent::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
}

void UStatusComponent::AddHealth(float InAmount)
{
	Health += InAmount;
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);
}

void UStatusComponent::SubHealth(float InAmount)
{
	Health -= InAmount;
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);
}

//void UStatusComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//	DOREPLIFETIME(UStatusComponent, Health);
//	DOREPLIFETIME(UStatusComponent, Shield);
//}

//void UStatusComponent::OnRep_Health(float LastHealth)
//{
//	//UpdateHUDHealth();
//	//Cast<UHealthWidget>(HealthWidget->GetUserWidgetObject())->Update(Status->GetHealth(), Status->GetMaxHealth());
//	if (Health < LastHealth)
//	{
//		//PlayHitReactMontage();
//	}
//}
//
//void UStatusComponent::OnRep_Shield(float LastShield)
//{
//	//UpdateHUDShield();
//	if (Shield < LastShield)
//	{
//		//PlayHitReactMontage();
//	}
//}