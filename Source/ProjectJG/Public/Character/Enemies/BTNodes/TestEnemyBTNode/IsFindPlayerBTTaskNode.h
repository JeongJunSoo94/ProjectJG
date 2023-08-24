// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "IsFindPlayerBTTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTJG_API UIsFindPlayerBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UIsFindPlayerBTTaskNode();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnGameplayTaskActivated(UGameplayTask& Task);
};
