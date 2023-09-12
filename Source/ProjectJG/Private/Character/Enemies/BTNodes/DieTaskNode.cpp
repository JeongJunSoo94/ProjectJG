#include "Character/Enemies/BTNodes/DieTaskNode.h"
#include "Global.h"
#include "Character/Enemies/MeleeEnemy/MeleeEnemyCharacter.h"
#include "Character/Enemies/MeleeEnemy/MeleeEnemyAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Character/Components/StatusComponent.h"

UDieTaskNode::UDieTaskNode()
{
	NodeName = TEXT("Die");
}

EBTNodeResult::Type UDieTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* ThisBlackboardComp = OwnerComp.GetAIOwner()->GetBlackboardComponent();
	AMeleeEnemyCharacter* ThisAICharacter = Cast<AMeleeEnemyCharacter>(ThisBlackboardComp->GetValueAsObject("SelfActor"));

	UStatusComponent* status = Cast<UStatusComponent>(ThisBlackboardComp->GetValueAsObject("Status"));
	if (status == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	if (status->GetHealth()<=0)
	{
		ThisAICharacter->Die();
		return EBTNodeResult::Aborted;
	}

	return EBTNodeResult::Failed;
}
