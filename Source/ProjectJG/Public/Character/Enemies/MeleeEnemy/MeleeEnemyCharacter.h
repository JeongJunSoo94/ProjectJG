#pragma once

#include "CoreMinimal.h"
#include "Character/Enemies/BaseEnemyCharacter.h"
#include "Character/Interface/MeleeBehaviorTreeInterface.h"
#include "Character/Interface/ActionNotifiable.h"
#include "Character/Components/CActionComponent.h"
#include "Character/Enemies/MeleeEnemy/MeleeEnemyAIController.h"
#include "Character/Interface/CollisionStateNotifiable.h"
#include "MeleeEnemyCharacter.generated.h"

UCLASS()
class PROJECTJG_API AMeleeEnemyCharacter : public ABaseEnemyCharacter,public IMeleeBehaviorTreeInterface , public IActionNotifiable,public ICollisionStateNotifiable
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Melee")
		TSubclassOf<AMeleeEnemyAIController> aicontroller;
	UPROPERTY(VisibleDefaultsOnly, Category = "Melee")
		class UAnimMontage* DeathMontage;
	UPROPERTY(VisibleDefaultsOnly)
		class UMeleeAttackActionComponent* MeleeActionComponent;
	UPROPERTY(VisibleDefaultsOnly, Category = "Melee")
		class UBoxComponent* WeaponCollisionBox;
	UPROPERTY(VisibleDefaultsOnly, Category = "Melee")
		FName WeaponCollisionSocket = "Muzzle_Front";
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:
	AMeleeEnemyCharacter();

protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
	virtual void MeleeAttack()override;
	virtual void Die()override;
	virtual void RegistBlackBoardDatas(class UBlackboardComponent* blackboard)override;
private:
	UCActionComponent* GetActionComponent();
	virtual void BeginNotifyAction()override;
	virtual void MiddleNotifyAction()override;
	virtual void EndNotifyAction()override;
	virtual void CollisionStateNotifyBegin()override;
	virtual void CollisionStateNotifyEnd() override;
public:
	bool isAttacked;
};
