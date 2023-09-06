#include "Character/Components/CActionComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"

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

void UCActionComponent::OnStartAction()
{
}

void UCActionComponent::BeginNotifyAction()
{
}
void UCActionComponent::MiddleNotifyAction()
{
}
void UCActionComponent::EndNotifyAction()
{
}
void UCActionComponent::OnEndAction()
{
}