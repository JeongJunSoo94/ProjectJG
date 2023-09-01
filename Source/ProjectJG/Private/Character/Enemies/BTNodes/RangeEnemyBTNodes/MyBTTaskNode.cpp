// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemies/BTNodes/RangeEnemyBTNodes/MyBTTaskNode.h"
#include "Global.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "Character/CBaseCharacter.h"

UMyBTTaskNode::UMyBTTaskNode()
{

}

EBTNodeResult::Type UMyBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (BBComp == nullptr)
		return EBTNodeResult::Failed;

	AActor* targetActor = Cast<AActor>(BBComp->GetValueAsObject(TEXT("TargetActor")));
	if (targetActor == nullptr)
		return EBTNodeResult::Failed;

	AActor* owner = OwnerComp.GetOwner();
	if (owner == nullptr)
		return EBTNodeResult::Failed;

	Clog::Log(owner->GetName() + " Get " + targetActor->GetName());

	return EBTNodeResult::Succeeded;
}

void UMyBTTaskNode::OnGameplayTaskActivated(UGameplayTask& Task)
{

}
