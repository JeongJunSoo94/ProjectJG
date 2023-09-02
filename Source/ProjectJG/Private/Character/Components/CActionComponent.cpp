#include "Character/Components/CActionComponent.h"
#include "Global.h"
#include "Character/CBaseCharacter.h"
#include "GameFramework/Character.h"

UCActionComponent::UCActionComponent()
{

}


void UCActionComponent::BeginPlay()
{
	Super::BeginPlay();

}


void UCActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCActionComponent::SetOwnerCharacter(ACharacter* character)
{
}

void UCActionComponent::OnAction()
{
}

void UCActionComponent::BeginAction()
{
}
void UCActionComponent::EndAction()
{
}