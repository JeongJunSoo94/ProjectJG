// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Enemies/BaseEnemyCharacter.h"
#include "Character/Interface/ActionNotifiable.h"
#include "RangeEnemyCharacter.generated.h"

/**
 * 
 */
namespace BlackboardDataNames
{
	const FName SelfActor = TEXT("SelfActor");
	const FName TargetActor = TEXT("TargetActor");
	const FName SquardDistanceToTarget = TEXT("SquardDistanceToTarget");
	const FName CharacterStatus = TEXT("CharacterStatus");

}

UCLASS()
class PROJECTJG_API ARangeEnemyCharacter : public ABaseEnemyCharacter, public IActionNotifiable
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "TriggerSphere")
		class USphereComponent* Sphere;
	UFUNCTION()
		void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleDefaultsOnly, Category = "EnemyWeapon")
		class URangeEnemyWeaponComponent* Weapon;

	UPROPERTY(VisibleDefaultsOnly, Category = "Hit_Material")
		class UMaterial* Hit_material;

	const FName frontDieMontage = TEXT("ForwardDie");
	const FName backDieMontage = TEXT("BackwardDie");

public:
	ARangeEnemyCharacter();

protected:
	virtual void BeginPlay() override;
	TArray<class UMaterialInstanceDynamic*> HitMaterial_Dynamics;
public:
	virtual void Tick(float DeltaTime) override;
	//virtual void BeginHitEffect(AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit) override;
	//virtual void SetImpactVectorFrom(FVector& ProjectileVector) override;

	virtual void RegistBlackBoardDatas(class UBlackboardComponent* blackboard) override;

	AActor* GetPriorityTarget();
	void RayToPlayer();
	void Fire();

	virtual void Die() override;

	void BeginNotifyAction();
	void MiddleNotifyAction();
	void EndNotifyAction();

protected:
	double MaxRemainSquaredDistance = 0.25f;
	bool IsDoEffect = false;
	FVector HitImpact;

	void DoEffect();

	TMap<FName,UAnimMontage*> BaseMontages;
	TMap<AActor*, int> PlayerAgrroMap;
	FTimerHandle RayTimer;
	FCollisionQueryParams CollisionParams;
	FTimerHandle EffectTimer;
	float EffectValue;

	bool IsHitFromForward = false;
};
