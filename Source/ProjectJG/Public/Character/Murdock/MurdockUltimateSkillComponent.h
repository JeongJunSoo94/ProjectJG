
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MurdockUltimateSkillComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTJG_API UMurdockUltimateSkillComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "UltimateAnim")
		class UAnimMontage* UltimateAnim;
	UPROPERTY(VisibleDefaultsOnly, Category = "UltimateAnim")
		class UAnimMontage* UltimateLoopAnim;


	UPROPERTY(VisibleDefaultsOnly, Category = "Ultimate")
		class UParticleSystem* UltimateShoot;

	UPROPERTY(VisibleDefaultsOnly, Category = "Ultimate")
		class UParticleSystemComponent* UltimateParticle;

	UPROPERTY(VisibleDefaultsOnly)
		class ACBaseCharacter* Ownercharacter;
public:	
	UMurdockUltimateSkillComponent();

	void BeginUltimate();
	void LoopUltimate();
	void EndUltimate();

	
protected:
	virtual void BeginPlay() override;
		
};
