// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemies/BTNodes/RangeEnemyBTNodes/BTTaskNode_IsTarget.h"
#include "Global.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "Character/CBaseCharacter.h"

UBTTaskNode_IsTarget::UBTTaskNode_IsTarget()
{

}

EBTNodeResult::Type UBTTaskNode_IsTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Clog::Log("IstargetNode");

	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (BBComp == nullptr)
		return EBTNodeResult::Failed;

	AActor* targetActor = Cast<AActor>(BBComp->GetValueAsObject(TEXT("TargetActor")));
	if (targetActor == nullptr)
	{
		//Clog::Log(targetActor);
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Succeeded;
}

void UBTTaskNode_IsTarget::OnGameplayTaskActivated(UGameplayTask& Task)
{

}