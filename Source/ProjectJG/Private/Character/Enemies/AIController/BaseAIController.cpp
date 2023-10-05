#include "Character/Enemies/AIController/BaseAIController.h"
#include "Global.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Enemies/BaseEnemyCharacter.h"

ABaseAIController::ABaseAIController()
{
	UBlackboardComponent* blackboard;
	CHelpers::CreateActorComponent<UBlackboardComponent>(this, &blackboard, "BlackboardComponent");
	Blackboard = blackboard;
	BlackboardComp = blackboard;
}

void ABaseAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	CheckNull(BTree);
	CheckNull(BlackboardDataAsset);
	if (UseBlackboard(BlackboardDataAsset, BlackboardComp))
	{
		ABaseEnemyCharacter* character = Cast<ABaseEnemyCharacter>(InPawn);
		CheckNull(character);
		character->RegistBlackBoardDatas(BlackboardComp);
		if (!RunBehaviorTree(BTree))
		{
		}
		BTreeComp = Cast<UBehaviorTreeComponent>(BrainComponent);
	}
}

void ABaseAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ABaseAIController::StartAI()
{
	if (BTreeComp != nullptr)
	{
		BTreeComp->StartTree(*this->BTree, EBTExecutionMode::Looped);
		Initialize();
	}
}
void ABaseAIController::StopAI()
{
	if (BTreeComp != nullptr)
	{
		BTreeComp->StopTree(EBTStopMode::Safe);
	}
}

void ABaseAIController::Initialize()
{
}