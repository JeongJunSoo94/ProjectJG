#include "Character/Enemies/MeleeEnemy/MeleeEnemyAIController.h"
#include "Global.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

AMeleeEnemyAIController::AMeleeEnemyAIController()
{
	UBlackboardComponent* blackboard;
	CHelpers::CreateActorComponent<UBlackboardComponent>(this, &blackboard, "BlackboardComponent");
	Blackboard = blackboard;
	BlackboardComp = blackboard;

	CHelpers::GetAsset<UBehaviorTree>(&BT, "BehaviorTree'/Game/Developers/JJS/Enemy/MeleeEnemyBehaviorTree.MeleeEnemyBehaviorTree'");
	
	CHelpers::GetAsset<UBlackboardData>(&BDAsset, "BlackboardData'/Game/Developers/JJS/Enemy/MeleeEnemyBlackBoard.MeleeEnemyBlackBoard'");

}
//
void AMeleeEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	//if (RunBehaviorTree(BT))
	//{
	//}
}

void AMeleeEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (UseBlackboard(BDAsset, BlackboardComp))
	{
		if (!RunBehaviorTree(BT))
		{
			Clog::Log("No");
		}
	}
}