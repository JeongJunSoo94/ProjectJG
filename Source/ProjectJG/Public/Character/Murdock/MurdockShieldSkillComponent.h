
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MurdockShieldSkillComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTJG_API UMurdockShieldSkillComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "ShieldAnim")
		class UAnimMontage* ShieldAnim;

	UPROPERTY(VisibleDefaultsOnly, Category = "Shield")
		class UParticleSystem* Shield;
	UPROPERTY(VisibleDefaultsOnly, Category = "Shield")
		class UParticleSystem* ShieldBreak;


	UPROPERTY(VisibleDefaultsOnly, Category = "Shield")
		class UParticleSystemComponent* ShieldParticle;

	UPROPERTY(VisibleDefaultsOnly)
		class ACBaseCharacter* Ownercharacter;

public:	
	UMurdockShieldSkillComponent();

	void BeginShield();
	void BreakShield();

	void LoopShieldMontage();
protected:
	virtual void BeginPlay() override;

};
