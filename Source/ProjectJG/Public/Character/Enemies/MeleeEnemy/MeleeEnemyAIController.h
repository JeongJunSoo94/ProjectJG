#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MeleeEnemyAIController.generated.h"

UCLASS()
class PROJECTJG_API AMeleeEnemyAIController : public AAIController
{
	GENERATED_BODY()
protected:
		UPROPERTY(VisibleDefaultsOnly, Category = "BT")
			class UBehaviorTree* BT;
public:
	AMeleeEnemyAIController();
protected:
	virtual void BeginPlay() override;

};
