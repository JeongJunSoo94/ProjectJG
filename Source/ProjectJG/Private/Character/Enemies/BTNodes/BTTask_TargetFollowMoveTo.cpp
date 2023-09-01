#include "Character/Enemies/BTNodes/BTTask_TargetFollowMoveTo.h"
#include "Global.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "AISystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "VisualLogger/VisualLogger.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Tasks/AITask_MoveTo.h"
#include "NavigationSystem.h"

UBTTask_TargetFollowMoveTo::UBTTask_TargetFollowMoveTo()
{
	NodeName = TEXT("FollowMoveTo");
}

EBTNodeResult::Type UBTTask_TargetFollowMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ThisTree == nullptr || ThisController == nullptr || ThisAICharacter == nullptr)
	{
		ThisTree = OwnerComp.GetCurrentTree();
		ThisController = Cast<AAIController>(OwnerComp.GetAIOwner());
		ThisAICharacter = Cast<AActor>(ThisController->GetPawn());

		if (ThisTree == nullptr || ThisController == nullptr || ThisAICharacter == nullptr)
		{
			Clog::Log("AI null");
			return EBTNodeResult::Failed;
		}
	}
	UNavigationSystemV1* navSystem = UNavigationSystemV1::GetNavigationSystem(ThisAICharacter->GetWorld());

	if(navSystem==nullptr)
		return EBTNodeResult::Failed;
	Clog::Log("Find");
	
	UAITask_MoveTo* AIMoveTask = NewBTAITask<UAITask_MoveTo>(OwnerComp);
	AIMoveTask->AIMoveTo(ThisController, ThisController->GetBlackboardComponent()->GetValueAsVector(TEXT("TargetLocation")), Cast<AActor>(ThisController->GetBlackboardComponent()->GetValueAsObject("TargetFollow")), NULL);

	return EBTNodeResult::Succeeded;
}