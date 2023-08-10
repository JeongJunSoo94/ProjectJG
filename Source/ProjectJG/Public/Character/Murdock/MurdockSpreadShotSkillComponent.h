
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MurdockSpreadShotSkillComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTJG_API UMurdockSpreadShotSkillComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "ShotGunAnim")
		class UAnimMontage* SpreadShotAnim;

	UPROPERTY(VisibleDefaultsOnly, Category = "ShotGun")
		class UParticleSystem* SpreadShotFlash;
	UPROPERTY(VisibleDefaultsOnly, Category = "ShotGun")
		class UParticleSystem* SpreadShotImpact;

	UPROPERTY(VisibleDefaultsOnly, Category = "ShotGun")
		class UParticleSystemComponent* SpreadShotParticle;

	UPROPERTY(VisibleDefaultsOnly, Category = "ShotGun")
		TSubclassOf<class AMurdockSpreadShotBullet> BulletClass;
	UPROPERTY(VisibleDefaultsOnly)
		class UObjectPoolFactory* ObjectPoolFactory;
	UPROPERTY(VisibleDefaultsOnly)
		class ACBaseCharacter* OwnerCharacter;

	UFUNCTION()
		void OnHitPaticle(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:	
	UMurdockSpreadShotSkillComponent();

	void ZoomInSpreadShot();
	void ShootSpreadShot();

	void LoopZoomMontage();
	
	void CreateObjectPool();

protected:
	virtual void BeginPlay() override;



		
};
