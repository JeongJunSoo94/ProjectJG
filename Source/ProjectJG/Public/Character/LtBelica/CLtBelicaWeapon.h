#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CLtBelicaWeapon.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTJG_API UCLtBelicaWeapon : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "SMG")
		class UAnimMontage* FireMontage;
	UPROPERTY(VisibleDefaultsOnly, Category = "SMG")
		class UParticleSystem* FlashParticle;
	UPROPERTY(VisibleDefaultsOnly, Category = "SMG")
		class UParticleSystem* EjectParticle;
	UPROPERTY(VisibleDefaultsOnly, Category = "SMG")
		int32 MuzzleIndex;

	UPROPERTY(VisibleDefaultsOnly, Category = "SMG")
		class USoundCue* FireSoundCue;
	UPROPERTY(VisibleDefaultsOnly, Category = "SMG")
		TSubclassOf<class ACBullet> BulletClass;
	UPROPERTY(VisibleDefaultsOnly, Category = "SMG")
		class UParticleSystem* ImpactParticle;
	UPROPERTY(VisibleDefaultsOnly, Category = "SMG")
		class UMaterialInstanceConstant* DecalMaterial;

	UFUNCTION()
		void OnHitPaticle(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UPROPERTY(VisibleDefaultsOnly)
		class UObjectPoolFactory* ObjectPoolFactory;
public:	
	UCLtBelicaWeapon();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	class ACBaseCharacter* OwnerCharacter;
public:
	void SetOwnerCharacter(ACBaseCharacter* character);

	void Begin_Fire();
	void Firing();
	void End_Fire();

private:
	FTimerHandle Timer;
	bool IsEquipped;
	bool IsFiring;
public:
	bool GetIsFiring();
};
