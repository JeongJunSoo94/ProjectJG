
#pragma once

#include "CoreMinimal.h"
#include "Character/Components/CActionComponent.h"
#include "MurdockUltimateSkillComponent.generated.h"

UENUM(BlueprintType)
enum class UltimateSkillState : uint8
{
	ESkillReady = 0		UMETA(DisplayName = "Ready"),
	ESkillProceeding	UMETA(DisplayName = "Proceed"),
	ESkillStop			UMETA(DisplayName = "Stop"),
	EMAX				UMETA(DisplayName = "Max")
};


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

	UFUNCTION()
		void EndZoomLag();

	virtual void OnStartAction() override;
	virtual void OnEndAction() override;
	virtual void BeginNotifyAction() override;
	virtual void MiddleNotifyAction() override;
	virtual void EndNotifyAction() override;

	// move to protected member after Get JJs Notify interface  
protected:
	void BeginUltimate();
	void LoopUltimate();
	void EndUltimate();
	void ChangeEndState();

	void ChargeLaser();
	void ShotLaser();

	virtual void BeginPlay() override;
	UltimateSkillState curState = UltimateSkillState::ESkillReady;
private:
	int maxLaserCount = 3;
	int currentLaserCount = 0;
};
