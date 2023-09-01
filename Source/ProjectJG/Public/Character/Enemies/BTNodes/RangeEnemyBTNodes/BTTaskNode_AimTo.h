// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTaskNode_AimTo.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTJG_API UBTTaskNode_AimTo : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTaskNode_AimTo();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnGameplayTaskActivated(UGameplayTask& Task);
};
