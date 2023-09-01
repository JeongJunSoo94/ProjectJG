// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "InputCoreTypes.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_TargetDistance.generated.h"

/**
 * 
 */

UCLASS()
class PROJECTJG_API UBTService_TargetDistance : public UBTService_BlackboardBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = Blackboard)
		struct FBlackboardKeySelector SquaredDistanceKey;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	
public:
	UBTService_TargetDistance();
};
