// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemies/AIController/TestAIController.h"
#include "Global.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"


ATestAIController::ATestAIController()
{
	UBlackboardComponent* blackboard;
	CHelpers::CreateActorComponent<UBlackboardComponent>(this, &blackboard, "Blackboard");
	Blackboard = blackboard;

	CHelpers::GetAsset<UBehaviorTree>(&BTAsset, "BehaviorTree'/Game/Developers/GohyeongJu/Characters/Enemy/EnemyBehivorTrees/EnemyTestBehaviorTree.EnemyTestBehaviorTree'");

}

void ATestAIController::BeginPlay()
{
	RunBehaviorTree(BTAsset);


}

