#include "Character/Enemies/RangeEnemy/RangeEnemyAIController.h"
#include "Global.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"

ARangeEnemyAIController::ARangeEnemyAIController()
{
	CHelpers::GetAsset<UBehaviorTree>(&BTree, "BehaviorTree'/Game/Developers/GohyeongJu/Characters/Enemy/RangeEnemy/BehaviorTree/RangeBehaviorTree.RangeBehaviorTree'");
	CHelpers::GetAsset<UBlackboardData>(&BlackboardDataAsset, "BlackboardData'/Game/Developers/GohyeongJu/Characters/Enemy/RangeEnemy/BehaviorTree/RangeBlackboardData.RangeBlackboardData'");
}

void ARangeEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ARangeEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ARangeEnemyAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ARangeEnemyAIController::StartAI()
{
	Super::StartAI();
}
void ARangeEnemyAIController::StopAI()
{
	Super::StopAI();
}