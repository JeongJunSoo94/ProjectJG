#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "FindBTTaskNode.generated.h"

UCLASS()
class PROJECTJG_API UFindBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UFindBTTaskNode();
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp,uint8* NodeMemory);
	virtual void OnGameplayTaskActivated(UGameplayTask& Task)override;
};
