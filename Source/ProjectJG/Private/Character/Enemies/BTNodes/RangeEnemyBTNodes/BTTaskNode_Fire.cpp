// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemies/BTNodes/RangeEnemyBTNodes/BTTaskNode_Fire.h"
#include "Global.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Character/Enemies/RangeEnemy/RangeEnemyCharacter.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"


UBTTaskNode_Fire::UBTTaskNode_Fire()
{
	NodeName = "Fire";
}

EBTNodeResult::Type UBTTaskNode_Fire::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	AAIController* AI = OwnerComp.GetAIOwner();
	if (AI == nullptr)
	{
		Clog::Log("AI is null");
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (BBComp == nullptr)
	{
		Clog::Log("BlackboardComponent is null");
		return EBTNodeResult::Failed;
	}

	ARangeEnemyCharacter* SelfActor = Cast<ARangeEnemyCharacter>(BBComp->GetValueAsObject("SelfActor"));

	SelfActor->Fire();

	return EBTNodeResult::Succeeded;


}

void UBTTaskNode_Fire::OnGameplayTaskActivated(UGameplayTask& Task)
{

}
