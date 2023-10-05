#include "Character/Enemies/MeleeEnemy/MeleeEnemyAIController.h"
#include "Global.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

AMeleeEnemyAIController::AMeleeEnemyAIController()
{
	CHelpers::GetAsset<UBehaviorTree>(&BTree, "BehaviorTree'/Game/Developers/JJS/Enemy/MeleeEnemyBehaviorTree.MeleeEnemyBehaviorTree'");
	CHelpers::GetAsset<UBlackboardData>(&BlackboardDataAsset, "BlackboardData'/Game/Developers/JJS/Enemy/MeleeEnemyBlackBoard.MeleeEnemyBlackBoard'");
}
void AMeleeEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AMeleeEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AMeleeEnemyAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AMeleeEnemyAIController::StartAI()
{
	Super::StartAI();
}
void AMeleeEnemyAIController::StopAI() 
{
	Super::StopAI();
}

void AMeleeEnemyAIController::Initialize()
{
	BlackboardComp->SetValueAsBool(TEXT("IsAttacked"), false);
	BlackboardComp->SetValueAsObject(TEXT("TargetFollow"), nullptr);
}