
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RangeEnemyWeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTJG_API URangeEnemyWeaponComponent : public UActorComponent
{
	GENERATED_BODY()
protected:
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


	class ABaseEnemyCharacter* OwnerCharacter = nullptr;
public:	
	URangeEnemyWeaponComponent();

	void Fire();
	void CreateObjectPool();
protected:
	
	virtual void BeginPlay() override;

};
