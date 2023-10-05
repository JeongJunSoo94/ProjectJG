// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Enemies/AIController/BaseAIController.h"
#include "RangeEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTJG_API ARangeEnemyAIController : public ABaseAIController
{
	GENERATED_BODY()
public:
	ARangeEnemyAIController();
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void StartAI()override;
	virtual void StopAI()override;
protected:
	virtual void BeginPlay() override;
};
