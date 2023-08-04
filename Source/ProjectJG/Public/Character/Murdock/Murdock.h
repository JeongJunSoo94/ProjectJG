// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CBaseCharacter.h"
#include "Murdock.generated.h"

UCLASS()
class PROJECTJG_API AMurdock : public ACBaseCharacter
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleDefaultsOnly)
		class UMurdockWeapon* MurdockWeapon;
public:
	AMurdock();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void OnFire();
	void OffFire();

public:
	void EndShotGunFireAnim();
	bool GetLtBelicaWeaponIsFiring();
	bool GetLtBelicaIsAbiliting();
};
