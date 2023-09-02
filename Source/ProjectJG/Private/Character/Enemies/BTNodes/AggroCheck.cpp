#include "Character/Enemies/BTNodes/AggroCheck.h"
#include "Global.h"
#include "Character/Enemies/MeleeEnemy/MeleeEnemyCharacter.h"
#include "Character/Enemies/MeleeEnemy/MeleeEnemyAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Character/CBaseCharacter.h"

UAggroCheck::UAggroCheck()
{
	Interval = 1.0f;
}

void UAggroCheck::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* ThisBlackboardComp = OwnerComp.GetAIOwner()->GetBlackboardComponent();
	ABaseEnemyCharacter* ThisAICharacter = Cast<ABaseEnemyCharacter>(ThisBlackboardComp->GetValueAsObject("SelfActor"));

	if (ThisBlackboardComp == nullptr ||  ThisAICharacter == nullptr)
	{
		return;
	}

	FCollisionQueryParams SphereSweepParams(FName(TEXT("AggroCheck")), true, ThisAICharacter);
	//FCollisionQueryParams ObjectQuery(ECC_GameTraceChannel1);

	FHitResult SphereHitOut(ForceInit);
	
	//DrawDebugSphere(ThisAICharacter->GetWorld(),ThisAICharacter->GetActorLocation(),1500,12,FColor::Red,false,4.0f);

	bool bResult = ThisAICharacter->GetWorld()->SweepSingleByObjectType(SphereHitOut, ThisAICharacter->GetActorLocation(),
		ThisAICharacter->GetActorLocation() + FVector(0.0f, 0.0f, 10.0f), FQuat(), ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(1500), SphereSweepParams);

	if (bResult)
	{
		FHitResult HitOut(ForceInit);
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(ThisAICharacter);
		if (SphereHitOut.GetActor() == nullptr)
		{
			Clog::Log(ThisAICharacter);
			Clog::Log("SphereHitOut null");
			return;
		}
		//DrawDebugLine(GetWorld(), ThisAICharacter->GetActorLocation(), SphereHitOut.GetActor()->GetActorLocation(), FColor::Red, false, 1, 0, 1);
		bool IsHit = GetWorld()->LineTraceSingleByChannel(HitOut, ThisAICharacter->GetActorLocation(), SphereHitOut.GetActor()->GetActorLocation(), ECC_WorldDynamic, CollisionParams);
		
		if (IsHit)
		{
			ACBaseCharacter* player = Cast<ACBaseCharacter>(HitOut.GetActor());
			if (player == nullptr)
			{
				ThisBlackboardComp->SetValueAsObject(TEXT("TargetFollow"), nullptr);
				ThisBlackboardComp->SetValueAsFloat(TEXT("Distance"), 0.0f);
				return;
			}
			ThisBlackboardComp->SetValueAsObject(TEXT("TargetFollow"), SphereHitOut.GetActor());
			ThisBlackboardComp->SetValueAsVector(TEXT("HomeLocation"), ThisAICharacter->GetActorLocation());
			ThisBlackboardComp->SetValueAsVector(TEXT("TargetLocation"), SphereHitOut.GetActor()->GetActorLocation());
			ThisBlackboardComp->SetValueAsFloat(TEXT("Distance"), HitOut.Distance);
		}
		else
		{
			ThisBlackboardComp->SetValueAsObject(TEXT("TargetFollow"), nullptr);
		}
	}
	else
	{
		ThisBlackboardComp->SetValueAsObject(TEXT("TargetFollow"), nullptr);
	}
}
