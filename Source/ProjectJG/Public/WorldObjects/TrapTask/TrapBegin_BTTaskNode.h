// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TrapBegin_BTTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTJG_API UTrapBegin_BTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UTrapBegin_BTTaskNode();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnGameplayTaskActivated(UGameplayTask& Task);
};
