#pragma once

#include "CoreMinimal.h"
#include "Character/CBaseCharacter.h"
#include "CLtBelica.generated.h"

UCLASS()
class PROJECTJG_API ACLtBelica : public ACBaseCharacter
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCLtBelicaWeapon* LtBelicaWeapon;
	UPROPERTY(VisibleDefaultsOnly)
		class UCQAbliltyActionComponent* LtBelicaQAbility;
public:
	ACLtBelica();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void OnFire();
	void OffFire();
	void OnQAbility();
public:
	bool GetLtBelicaWeaponIsFiring();
	bool GetLtBelicaIsAbiliting();
};