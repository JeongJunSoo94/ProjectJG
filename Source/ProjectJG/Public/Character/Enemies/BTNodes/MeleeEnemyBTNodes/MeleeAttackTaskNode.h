#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MeleeAttackTaskNode.generated.h"

UCLASS()
class PROJECTJG_API UMeleeAttackTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UMeleeAttackTaskNode();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,float DeltaSeconds)override;
};
