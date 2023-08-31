// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemies/BTNodes/RangeEnemyBTNodes/BTTaskNode_AimTo.h"
#include "Global.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Character/Enemies/RangeEnemy/RangeEnemyCharacter.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"


UBTTaskNode_AimTo::UBTTaskNode_AimTo()
{
	NodeName = "Aim To";
}

EBTNodeResult::Type UBTTaskNode_AimTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		UObject* KeyValue = BBComp->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
		AActor* TargetActor = Cast<AActor>(KeyValue);

		if (AI->GetFocusActor() != TargetActor)
		{
			AI->SetFocus(TargetActor,EAIFocusPriority::LastFocusPriority);
		}
		return EBTNodeResult::Succeeded;
	}
	else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		const FVector TargetLocation = BBComp->GetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID());
		
		AI->SetFocalPoint(TargetLocation);
		return EBTNodeResult::Succeeded;
	}
	else
	{
		return EBTNodeResult::Failed;
	}

	
}

void UBTTaskNode_AimTo::OnGameplayTaskActivated(UGameplayTask& Task)
{

}

