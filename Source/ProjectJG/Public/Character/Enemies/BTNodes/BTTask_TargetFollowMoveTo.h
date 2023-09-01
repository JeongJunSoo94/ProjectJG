#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TargetFollowMoveTo.generated.h"

UCLASS()
class PROJECTJG_API UBTTask_TargetFollowMoveTo : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_TargetFollowMoveTo();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
private:
	FBlackboardKeySelector player;
	UBehaviorTree* ThisTree;
	class AAIController* ThisController;
	class AActor* ThisAICharacter;
};
