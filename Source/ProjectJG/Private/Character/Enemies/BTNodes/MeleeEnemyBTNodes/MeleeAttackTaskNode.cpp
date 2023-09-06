#include "Character/Enemies/BTNodes/MeleeEnemyBTNodes/MeleeAttackTaskNode.h"
#include "Global.h"
#include "Character/Enemies/MeleeEnemy/MeleeEnemyCharacter.h"
#include "Character/Enemies/MeleeEnemy/MeleeEnemyAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

UMeleeAttackTaskNode::UMeleeAttackTaskNode()
{
	bNotifyTick = true;
	NodeName = TEXT("MeleeAttack");
}

EBTNodeResult::Type UMeleeAttackTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* ThisBlackboardComp = OwnerComp.GetAIOwner()->GetBlackboardComponent();
	AMeleeEnemyCharacter* ThisAICharacter = Cast<AMeleeEnemyCharacter>(ThisBlackboardComp->GetValueAsObject("SelfActor"));

	if (ThisBlackboardComp == nullptr || ThisAICharacter == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	ThisAICharacter->MeleeAttack();
	ThisBlackboardComp->SetValueAsBool(TEXT("IsAttacked"), true);

	return EBTNodeResult::InProgress;
}

void UMeleeAttackTaskNode::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	UBlackboardComponent* ThisBlackboardComp = OwnerComp.GetAIOwner()->GetBlackboardComponent();
	AMeleeEnemyCharacter* ThisAICharacter = Cast<AMeleeEnemyCharacter>(ThisBlackboardComp->GetValueAsObject("SelfActor"));
	
	if (!ThisAICharacter->isAttacked)
	{
		ThisBlackboardComp->SetValueAsBool(TEXT("IsAttacked"), false);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}