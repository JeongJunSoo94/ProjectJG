
#include "Character/Enemies/BTNodes/RangeEnemyBTNodes/RangeEnemyBTService.h"
#include "Global.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Character/Enemies/RangeEnemy/RangeEnemyCharacter.h"

URangeEnemyBTService::URangeEnemyBTService()
{
	Interval = 1.0f;
}

void URangeEnemyBTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (BBComp == nullptr)
	{
		Clog::Log("BlackboardComponent is null");
		return;
	}
	

	ARangeEnemyCharacter* Owner = Cast<ARangeEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Owner == nullptr)
	{
		Clog::Log("Owner is null");
		return;
	}

	AActor* priorityTarget = Owner->GetPriorityTarget();
	//Clog::Log(priorityTarget);
	AActor* targetActor = Cast<AActor>(BBComp->GetValueAsObject(TEXT("TargetActor")));


	if (priorityTarget == targetActor)
	{
		//Clog::Log(priorityTarget);
		return;
	}


	BBComp->SetValueAsObject(TEXT("TargetActor"), priorityTarget);

}