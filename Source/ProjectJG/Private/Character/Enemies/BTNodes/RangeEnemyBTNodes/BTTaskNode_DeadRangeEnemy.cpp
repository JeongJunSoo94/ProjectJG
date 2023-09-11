// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemies/BTNodes/RangeEnemyBTNodes/BTTaskNode_DeadRangeEnemy.h"
#include "Global.h"
#include "AIController.h"
#include "Character/Enemies/RangeEnemy/RangeEnemyCharacter.h"
#include "Character/Components/StatusComponent.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"



UBTTaskNode_DeadRangeEnemy::UBTTaskNode_DeadRangeEnemy()
{
	NodeName = "Dead";
}

EBTNodeResult::Type UBTTaskNode_DeadRangeEnemy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Clog::Log("Dead Task");
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

	ARangeEnemyCharacter* OwnerCharacter = nullptr;
	UStatusComponent* status = Cast<UStatusComponent>(BBComp->GetValueAsObject(BlackboardDataNames::CharacterStatus));
	if (status == nullptr)
	{
		Clog::Log("statusComponent is null");
		OwnerCharacter = Cast<ARangeEnemyCharacter>(BBComp->GetValueAsObject(BlackboardDataNames::SelfActor));
		OwnerCharacter->RegistBlackBoardDatas(BBComp);
		status = Cast<UStatusComponent>(BBComp->GetValueAsObject(BlackboardDataNames::CharacterStatus));
	}

	if (status->GetHealth() <= 0.0f)
	{
		Clog::Log("Die");
		if(OwnerCharacter==nullptr)
			OwnerCharacter = Cast<ARangeEnemyCharacter>(BBComp->GetValueAsObject(BlackboardDataNames::SelfActor));
		OwnerCharacter->Die();
		AI->ClearFocus(EAIFocusPriority::LastFocusPriority);
		return EBTNodeResult::Aborted;
	}
	else
	{
		
		return EBTNodeResult::Failed;
	}


}

void UBTTaskNode_DeadRangeEnemy::OnGameplayTaskActivated(UGameplayTask& Task)
{
}
