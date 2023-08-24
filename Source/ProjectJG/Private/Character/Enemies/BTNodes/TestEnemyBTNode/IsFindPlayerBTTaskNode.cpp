// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemies/BTNodes/TestEnemyBTNode/IsFindPlayerBTTaskNode.h"
#include "Global.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Character/Enemies/NavTestAvatar/NavTestCharacter.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Character/CBaseCharacter.h"

UIsFindPlayerBTTaskNode::UIsFindPlayerBTTaskNode()
{
	NodeName = TEXT("IsFindPlayer");
}

EBTNodeResult::Type UIsFindPlayerBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	ANavTestCharacter* NavActor = Cast<ANavTestCharacter>(OwnerComp.GetOwner());

	if(NavActor == nullptr)
		return EBTNodeResult::Failed;


	if (NavActor->TargetActors.Num() == 0)
		return EBTNodeResult::Failed;
	
	//UBlackboardData* board = GetBlackboardAsset();
	
	UBlackboardComponent* blackboardComponent = OwnerComp.GetBlackboardComponent();
	
	
	Clog::Log(blackboardComponent);

	float agrroValue = 0.0f;
	ACBaseCharacter* priorityTarget = nullptr;
	bool IsFindPlayer = false;
	
	for (TPair<ACBaseCharacter*,LookPlayerState> &Elem : NavActor->TargetActors)
	{
		if (Elem.Value == LookPlayerState::ELook)
		{
			if (agrroValue < NavActor->ActorsAggro[Elem.Key])
			{
				agrroValue = NavActor->ActorsAggro[Elem.Key];
				priorityTarget = Elem.Key;
				IsFindPlayer = true;
			}
		}
	}
	
	if (IsFindPlayer)
	{
		//Clog::Log(priorityTarget);
		//FBlackboard::FKey keyId = blackboardComponent->GetKeyID("PriorityTarget");
		//UObject* object = blackboardComponent->GetValueAsObject("PriorityTarget");
		const FName valueName = TEXT("PriorityTarget");
		//blackboardComponent->SetValue<UBlackboardKeyType_Object>(keyId, priorityTarget);
		blackboardComponent->SetValueAsObject(valueName,priorityTarget);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}

void UIsFindPlayerBTTaskNode::OnGameplayTaskActivated(UGameplayTask& Task)
{

}
