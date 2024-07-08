#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseAIController.generated.h"

UCLASS()
class PROJECTJG_API ABaseAIController : public AAIController
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "BT")
		class UBehaviorTree* BTree;
	UPROPERTY(VisibleDefaultsOnly, Category = "BT")
		class UBlackboardComponent* BlackboardComp;
	UPROPERTY(EditAnywhere, Category = "BT")
		class UBlackboardData* BlackboardDataAsset;
	UPROPERTY(VisibleDefaultsOnly, Category = "BT")
		class UBehaviorTreeComponent* BTreeComp;
public:
	ABaseAIController();
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void StartAI();
	virtual void StopAI();
	virtual void Initialize();

	void SetHUDHealth(float Health, float MaxHealth);
	void SetHUDShield(float Shield, float MaxShield);
protected:
	virtual void BeginPlay() override;
};
