#include "WorldObjects/Traps/FindBTTaskNode.h"
#include "Global.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Character/CBaseCharacter.h"
#include "WorldObjects/Traps/TestTrapActor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Character/Enemies/MeleeEnemy/MeleeEnemyCharacter.h"

UFindBTTaskNode::UFindBTTaskNode()
{
	NodeName = TEXT("Find");
}

EBTNodeResult::Type UFindBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AMeleeEnemyCharacter* cont = Cast<AMeleeEnemyCharacter>(OwnerComp.GetOwner());

	UBlackboardData* data = GetBlackboardAsset();
	
	//for (auto d : data->GetKeys())
	//{
	//	Clog::Log(d.EntryName.ToString());
	//	Clog::Log(d.KeyType);
	//}
	if (cont!=nullptr)
	{
		Clog::Log(cont->GetFName().ToString());
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

void UFindBTTaskNode::OnGameplayTaskActivated(UGameplayTask& Task)
{
}

