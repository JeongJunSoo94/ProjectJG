#include "Character/Ememy/MeleeEnemyAIController.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"

AMeleeEnemyAIController::AMeleeEnemyAIController()
{
	UBlackboardComponent* blackboard;
	CHelpers::CreateActorComponent<UBlackboardComponent>(this, &blackboard, "Blackboard");
	Blackboard = blackboard;

	CHelpers::GetAsset<UBehaviorTree>(&BT, "BehaviorTree'/Game/Developers/USER/WorldObjects/TrapBehaviorTree.TrapBehaviorTree'");
}

void AMeleeEnemyAIController::BeginPlay()
{
	RunBehaviorTree(BT);
}