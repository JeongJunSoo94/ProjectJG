// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_DeadRangeEnemy.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTJG_API UBTTaskNode_DeadRangeEnemy : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTaskNode_DeadRangeEnemy();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnGameplayTaskActivated(UGameplayTask& Task);
};
