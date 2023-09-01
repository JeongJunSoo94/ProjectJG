// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemies/BTNodes/RangeEnemyBTNodes/BTTaskBlackboardBase_ClearFocus.h"
#include "Global.h"
#include "AIController.h"


UBTTaskBlackboardBase_ClearFocus::UBTTaskBlackboardBase_ClearFocus()
{
	NodeName = "ClearFocus";
}

EBTNodeResult::Type UBTTaskBlackboardBase_ClearFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AI = OwnerComp.GetAIOwner();
	if (AI == nullptr)
	{
		Clog::Log("AI is null");
		return EBTNodeResult::Failed;
	}
	AI->ClearFocus(EAIFocusPriority::LastFocusPriority);
	return EBTNodeResult::Succeeded;
}

void UBTTaskBlackboardBase_ClearFocus::OnGameplayTaskActivated(UGameplayTask& Task)
{

}