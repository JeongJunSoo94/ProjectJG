#include "Character/Components/CActionComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "Widgets/CharacterSkillWidget.h"
#include "TimerManager.h"

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

void UCActionComponent::CoolTimeUpdate()
{
	CurCoolTime += IntervalCoolTime;
	OnUpdateWidgetTimer.Execute(CurCoolTime, MaxCoolTime);
	if (CurCoolTime >= MaxCoolTime)
	{
		IsCoolTiming = false;
		CurCoolTime = 0.0f;
		GetWorld()->GetTimerManager().ClearTimer(CoolTimeHandle);
	}
}