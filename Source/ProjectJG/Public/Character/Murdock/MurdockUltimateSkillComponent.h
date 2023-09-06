
#pragma once

#include "CoreMinimal.h"
#include "Character/Components/CActionComponent.h"
#include "MurdockUltimateSkillComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTJG_API UMurdockUltimateSkillComponent : public UCActionComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "UltimateAnim")
		class UAnimMontage* UltimateAnim;
	UPROPERTY(VisibleDefaultsOnly, Category = "UltimateAnim")
		class UAnimMontage* UltimateLoopAnim;


	UPROPERTY(VisibleDefaultsOnly, Category = "Ultimate")
		class UParticleSystem* UltimateLoop;
	UPROPERTY(VisibleDefaultsOnly, Category = "Ultimate")
		class UParticleSystem* UltimateBeginShot;
	UPROPERTY(VisibleDefaultsOnly, Category = "Ultimate")
		class UParticleSystem* UltimateMuzzleShot;
	UPROPERTY(VisibleDefaultsOnly, Category = "Ultimate")
		class UParticleSystem* UltimateLaser;
	UPROPERTY(VisibleDefaultsOnly, Category = "Ultimate")
		class UParticleSystem* UltimateLaserSight;
	UPROPERTY(VisibleDefaultsOnly, Category = "Ultimate")
		class UParticleSystem* UltimateLaserImpact;

	UPROPERTY(VisibleDefaultsOnly, Category = "Ultimate")
		class UParticleSystemComponent* UltimateParticle;

	UPROPERTY(VisibleDefaultsOnly, Category = "Ultimate")
		class USoundCue* FireSoundCue;


	UPROPERTY(VisibleDefaultsOnly)
		class AMurdock* OwnerCharacter;
public:	
	UMurdockUltimateSkillComponent();

	void LoopUltimate();

	void ShotLaser();
	void ChargeLaser();
	UFUNCTION()
		void EndZoomLag();

	bool IsStopSkill;


	virtual void OnStartAction() override;
	virtual void OnEndAction() override;

	// move to protected member after Get JJs Notify interface  
	void BeginUltimate();
	void EndUltimate();
protected:
	virtual void BeginPlay() override;

private:
	int maxLaserCount = 3;
	int currentLaserCount = 0;
};
