#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MurdockWeapon.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTJG_API UMurdockWeapon : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "ShotGun")
		class UAnimMontage* FireMontage;
	UPROPERTY(VisibleDefaultsOnly, Category = "ShotGun")
		class UParticleSystem* FlashParticle;
	UPROPERTY(VisibleDefaultsOnly, Category = "ShotGun")
		class UParticleSystem* EjectParticle;
	UPROPERTY(VisibleDefaultsOnly, Category = "ShotGun")
		int32 MuzzleIndex;

	UPROPERTY(VisibleDefaultsOnly, Category = "ShotGun")
		class USoundCue* FireSoundCue;
	UPROPERTY(VisibleDefaultsOnly, Category = "ShotGun")
		TSubclassOf<class AMurdockBullet> BulletClass;
	UPROPERTY(VisibleDefaultsOnly, Category = "ShotGun")
		class UParticleSystem* ImpactParticle;
	UPROPERTY(VisibleDefaultsOnly, Category = "ShotGun")
		class UMaterialInstanceConstant* DecalMaterial;


	UFUNCTION()
		void OnHitPaticle(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UPROPERTY(VisibleDefaultsOnly)
		class UObjectPoolFactory* ObjectPoolFactory;
public:	

	UMurdockWeapon();

protected:
	virtual void BeginPlay() override;

private:
	class ACBaseCharacter* OwnerCharacter;
	FTimerHandle Timer;
	bool IsEquipped;
	bool IsFiring;
public:
	void SetOwnerCharacter(ACBaseCharacter* character);

	void Begin_Fire();
	void Firing();
	void End_Fire();

	bool GetIsFiring();

};
