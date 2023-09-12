#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "DieTaskNode.generated.h"

UCLASS()
class PROJECTJG_API UDieTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UDieTaskNode();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
