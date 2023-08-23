// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldObjects/TrapTask/TrapBegin_BTTaskNode.h"
#include "Global.h"

UTrapBegin_BTTaskNode::UTrapBegin_BTTaskNode()
{
	NodeName = TEXT("Trap____Begin");
}


EBTNodeResult::Type UTrapBegin_BTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Type();
}

void UTrapBegin_BTTaskNode::OnGameplayTaskActivated(UGameplayTask& Task)
{

}
