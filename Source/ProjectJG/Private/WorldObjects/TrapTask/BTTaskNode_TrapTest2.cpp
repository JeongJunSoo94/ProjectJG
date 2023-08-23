// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldObjects/TrapTask/BTTaskNode_TrapTest2.h"
#include "Global.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "WorldObjects/Traps/TestTrapActor.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Int.h"

UBTTaskNode_TrapTest2::UBTTaskNode_TrapTest2()
{
	NodeName = TEXT("Trap Test 2");
}


EBTNodeResult::Type UBTTaskNode_TrapTest2::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ATestTrapActor* Trap = Cast<ATestTrapActor>(OwnerComp.GetOwner());
	UBlackboardData* board = GetBlackboardAsset();

	
	
	//Clog::Log(Trap->GetName());

	//FName name = board->GetKey(1)->EntryName;
	//Clog::Log(name.ToString());
	//Clog::Log(board->GetKey(1)->KeyType);



	UBlackboardKeyType* key = board->GetKey(1)->KeyType;

	UBlackboardKeyType_Int* T = Cast<UBlackboardKeyType_Int>(board->GetKey(1)->KeyType);



	
	if (Trap->GetName() == "BP_TrapActor_C_1")
	{
		Clog::Log(Trap->GetName());
		int32 num = UBlackboardKeyType_Int::GetValue(T, NodeMemory);
		if (num == 0)
		{
			UBlackboardKeyType_Int::SetValue(T, NodeMemory, 1);
			Clog::Log(num);
		}
		else
		{
			UBlackboardKeyType_Int::SetValue(T, NodeMemory, 0);
			Clog::Log(num);
		}
		
		

		//T++;
	}
	else
	{
		/*
		Clog::Log(Trap->GetName());
		int32 num = UBlackboardKeyType_Int::GetValue(T, NodeMemory);
		Clog::Log(num);
		num = 1;
		*/
		//board->GetKey(1)
	}
	

	

	return EBTNodeResult::Succeeded;
}

void UBTTaskNode_TrapTest2::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Clog::Log("OnGameplayTaskActivated");
}
