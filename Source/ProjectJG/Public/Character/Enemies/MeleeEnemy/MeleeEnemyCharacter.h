#pragma once

#include "CoreMinimal.h"
#include "Character/Enemies/BaseEnemyCharacter.h"
#include "Character/Interface/MeleeBehaviorTreeInterface.h"
#include "Character/Interface/ActionNotifiable.h"
#include "MeleeEnemyCharacter.generated.h"

UCLASS()
class PROJECTJG_API AMeleeEnemyCharacter : public ABaseEnemyCharacter,public IMeleeBehaviorTreeInterface , public IActionNotifiable
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCActionComponent* CurrentActionComponent;
	UPROPERTY(VisibleDefaultsOnly, Category = "Melee")
		class UAnimMontage* MeleeMontage;
protected:
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
	virtual UCActionComponent* GetActionComponent() override;
public:
	bool isAttacked;
};
