
#pragma once

#include "CoreMinimal.h"
#include "Character/Components/CActionComponent.h"
#include "MurdockSpreadShotSkillComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTJG_API UMurdockSpreadShotSkillComponent : public UCActionComponent
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
		class AMurdock* OwnerCharacter;

	UPROPERTY(VisibleDefaultsOnly, Category = "ShotGun")
		class USoundCue* FireSoundCue;
	UPROPERTY(VisibleDefaultsOnly, Category = "ShotGun")
		class USoundCue* ExplosionSoundCue;

	UFUNCTION()
		void OnHitPaticle(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


public:	
	UMurdockSpreadShotSkillComponent();


	void LoopZoomMontage();
	
	void CreateObjectPool();

	virtual void OnStartAction() override;
	virtual void OnEndAction() override;

protected:
	void ZoomInSpreadShot();
	void ShootSpreadShot();

	virtual void BeginPlay() override;

		
};
