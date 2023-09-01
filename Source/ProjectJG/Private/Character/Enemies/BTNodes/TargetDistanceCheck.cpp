#include "Character/Enemies/BTNodes/TargetDistanceCheck.h"
#include "Global.h"
#include "Character/Enemies/MeleeEnemy/MeleeEnemyCharacter.h"
#include "Character/Enemies/MeleeEnemy/MeleeEnemyAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

void UTargetDistanceCheck::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (ThisTree == nullptr || ThisController == nullptr || ThisAICharacter == nullptr)
	{
		ThisTree = OwnerComp.GetCurrentTree();
		ThisController = Cast<AMeleeEnemyAIController>(OwnerComp.GetAIOwner());
		ThisAICharacter = Cast<ABaseEnemyCharacter>(ThisController->GetPawn());

		if (ThisTree == nullptr || ThisController == nullptr || ThisAICharacter == nullptr)
		{
			Clog::Log("AI null");
			return;
		}
	}

	FCollisionQueryParams CollisionParams(FName(TEXT("TargetDistanceCheck")), true, ThisAICharacter);
	//FCollisionQueryParams ObjectQuery(ECC_GameTraceChannel1);

	FHitResult HitOut(ForceInit);
	
	//DrawDebugSphere(ThisAICharacter->GetWorld(),ThisAICharacter->GetActorLocation(),1500,12,FColor::Red,false,4.0f);

	//DrawDebugLine(GetWorld(), OutStart, OutEnd, FColor::Red, false, 1, 0, 1);
	//FVector end = ThisController->GetBlackboardComponent()->GetValueAsVector("TargetLocation");
	//FVector direction = KismetMathLibrary::GetUnitDirection(ThisAICharacter->GetActorLocation(), end);
	//bool IsHit = GetWorld()->LineTraceSingleByChannel(HitOut, ThisAICharacter->GetActorLocation(), , ECC_Visibility, CollisionParams);

	/*if (IsHit)
	{
	}
	else
	{
	}*/
}

