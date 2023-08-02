#include "Character/Components/StatusComponent.h"

UStatusComponent::UStatusComponent()
{
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
