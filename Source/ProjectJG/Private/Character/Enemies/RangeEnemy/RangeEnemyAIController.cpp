// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemies/RangeEnemy/RangeEnemyAIController.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"

ARangeEnemyAIController::ARangeEnemyAIController()
{
	UBlackboardComponent* blackboard;
	CHelpers::CreateActorComponent<UBlackboardComponent>(this, &blackboard, "Blackboard");
	Blackboard = blackboard;

	CHelpers::GetAsset<UBehaviorTree>(&BT, "BehaviorTree'/Game/Developers/GohyeongJu/Characters/Enemy/RangeEnemy/BehaviorTree/RangeBehaviorTree.RangeBehaviorTree'");

}

void ARangeEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	Clog::Log(GetPawn());
	Clog::Log(BT);
	RunBehaviorTree(BT);
}
