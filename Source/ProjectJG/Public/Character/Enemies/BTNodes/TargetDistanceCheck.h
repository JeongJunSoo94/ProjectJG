#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "TargetDistanceCheck.generated.h"


UENUM()
namespace EDistanceOption
{
	enum Type
	{
		Set				UMETA(DisplayName = "Is Set"),
		NotSet			UMETA(DisplayName = "Is Not Set"),
	};
}

UCLASS()
class PROJECTJG_API UTargetDistanceCheck : public UBTService
{
	GENERATED_BODY()
protected:
	UPROPERTY(Category = Service, EditAnywhere,meta = (ClampMin = "0.0"))
		float FindTargetDistance;
	UPROPERTY(Category = Service, EditAnywhere)
		TEnumAsByte<EDistanceOption::Type> DistanceOption;
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
private:
	UBehaviorTree* ThisTree;
	class AMeleeEnemyAIController* ThisController;
	class ABaseEnemyCharacter* ThisAICharacter;
};
