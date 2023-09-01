// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RangeEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTJG_API ARangeEnemyAIController : public AAIController
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "BT")
		class UBehaviorTree* BT;
public:
	ARangeEnemyAIController();
protected:
	virtual void BeginPlay() override;
};
