// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Enemies/BaseEnemyCharacter.h"
#include "RangeEnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTJG_API ARangeEnemyCharacter : public ABaseEnemyCharacter
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "TriggerSphere")
		class USphereComponent* Sphere;
	UFUNCTION()
		void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleDefaultsOnly, Category = "EnemyFire")
		class UAnimMontage* FireMontage;
	UPROPERTY(VisibleDefaultsOnly, Category = "EnemyFire")
		class UParticleSystem* FlashParticle;
	//UPROPERTY(VisibleDefaultsOnly, Category = "EnemyFire")
		//class UParticleSystem* EjectParticle;
	UPROPERTY(VisibleDefaultsOnly, Category = "EnemyFire")
		class USoundCue* FireSoundCue;
	UPROPERTY(VisibleDefaultsOnly, Category = "EnemyFire")
		TSubclassOf<class ARangeEnemy_Bullet> BulletClass;
	UPROPERTY(VisibleDefaultsOnly, Category = "ShotGun")
		class UParticleSystem* ImpactParticle;
	
	UFUNCTION()
		void OnHitPaticle(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UPROPERTY(VisibleDefaultsOnly)
		class UObjectPoolFactory* ObjectPoolFactory;


	UPROPERTY(VisibleDefaultsOnly, Category = "Hit_Material")
		class UMaterial* Hit_material;

public:
	ARangeEnemyCharacter();

protected:
	virtual void BeginPlay() override;
	TArray<class UMaterialInstanceDynamic*> HitMaterial_Dynamics;

public:
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float Damage) override;

	AActor* GetPriorityTarget();
	void RayToPlayer();

	double MaxRemainSquaredDistance = 0.25f;

	void Fire();

	virtual void BeginHitEffect(FVector NormalImpulse, const FHitResult& Hit) override;
	void DoEffect();
	bool IsDoEffect = false;
	FVector HitImpact;
protected:
	TMap<AActor*, int> PlayerAgrroMap;
	FTimerHandle RayTimer;
	FCollisionQueryParams CollisionParams;
	FTimerHandle EffectTimer;
	float EffectValue;
};
