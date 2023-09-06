
#pragma once

#include "CoreMinimal.h"
#include "Character/Components/CActionComponent.h"
#include "MurdockShieldSkillComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTJG_API UMurdockShieldSkillComponent : public UCActionComponent
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

	virtual void OnStartAction() override;
	virtual void OnEndAction() override;

	void LoopShieldMontage();
protected:
	void BeginShield();
	void BreakShield();
	virtual void BeginPlay() override;


};
