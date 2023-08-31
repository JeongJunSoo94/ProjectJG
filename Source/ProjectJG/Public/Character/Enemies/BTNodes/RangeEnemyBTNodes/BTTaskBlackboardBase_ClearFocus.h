// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTaskBlackboardBase_ClearFocus.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTJG_API UBTTaskBlackboardBase_ClearFocus : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTaskBlackboardBase_ClearFocus();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnGameplayTaskActivated(UGameplayTask& Task);
};
