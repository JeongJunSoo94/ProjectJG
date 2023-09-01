#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "AggroCheck.generated.h"

UCLASS()
class PROJECTJG_API UAggroCheck : public UBTService
{
	GENERATED_BODY()
public:
	UAggroCheck();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
