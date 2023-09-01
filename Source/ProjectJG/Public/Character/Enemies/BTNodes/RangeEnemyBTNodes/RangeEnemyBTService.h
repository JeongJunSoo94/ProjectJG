
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "RangeEnemyBTService.generated.h"


UCLASS()
class PROJECTJG_API URangeEnemyBTService : public UBTService
{
	GENERATED_BODY()
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
public:
	URangeEnemyBTService();

protected:
	float CumulativeTime = 0.0f;
	float RateTime = 0.0f;

};
