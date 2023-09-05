// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemies/BTNodes/RangeEnemyBTNodes/BTService_TargetDistance.h"
#include "Global.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Character/Enemies/RangeEnemy/RangeEnemyCharacter.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"


void UBTService_TargetDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (BBComp == nullptr)
	{
		Clog::Log("BlackboardComponent is null");
		return;
	}

	AActor* SelfActor = Cast<AActor>(BBComp->GetValueAsObject("SelfActor"));
	
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		UObject* KeyValue = BBComp->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
		AActor* TargetActor = Cast<AActor>(KeyValue);
		if (TargetActor == nullptr)
		{
			Clog::Log("TargetActor is Null");
			return;
		}

		float SquardDistance = (SelfActor->GetActorLocation() - TargetActor->GetActorLocation()).SquaredLength();
		//Clog::Log(SquaredDistanceKey.SelectedKeyName.ToString());
		BBComp->SetValueAsFloat(SquaredDistanceKey.SelectedKeyName, SquardDistance);

		return;
	}
	//Clog::Log(SquaredDistanceKey.SelectedKeyType);
	//Clog::Log("blackBoardKey is Only Object Type and SquardDistanceKey is Only Float Type");

}

UBTService_TargetDistance::UBTService_TargetDistance()
{
	NodeName = "Limit distance Service";
}

