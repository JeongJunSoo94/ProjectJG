// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CBaseCharacter.h"
#include "Murdock.generated.h"

UENUM(BlueprintType)
enum class MurdockBehaviorState : uint8
{
	EIdle = 0		UMETA(DisplayName = "Idle"),
	EFire			UMETA(DisplayName = "Fire"),
	EShield			UMETA(DisplayName = "Shield"),
	ESpreadShot		UMETA(DisplayName = "SpreadShot"),
	EUltimate		UMETA(DisplayName = "Ultimate"),
	EMAX			UMETA(DisplayName = "Max")
};

UCLASS()
class PROJECTJG_API AMurdock : public ACBaseCharacter
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleDefaultsOnly)
		class UMurdockWeapon* MurdockWeapon;

	UPROPERTY(VisibleDefaultsOnly)
		class UMurdockUltimateSkillComponent* UltimateSkill;
	UPROPERTY(VisibleDefaultsOnly)
		class UMurdockShieldSkillComponent* ShieldSkill;
	UPROPERTY(VisibleDefaultsOnly)
		class UMurdockSpreadShotSkillComponent* SpreadShotSkill;
	UPROPERTY(VisibleDefaultsOnly)
		class UMurdockTazerTrapSkillComponent* TazerTrapSkill;
	MurdockBehaviorState BehaviorState;
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
	void OnAlt();
	void OffAlt();

	void OnShield();
	void OffShield();

	void OnSpreadShot();
	void OffSpreadShot();

	void OnUltimate();
	void OffUltimate();
	
	float FrontYaw;
public:
	void LoopShield();
	void LoopSpreadShotZoom();
	void LoopUltimate();
	void EndUltimateToIdle();
	MurdockBehaviorState GetBehaviorState() { return BehaviorState; }

	float GetFrontYaw() { return FrontYaw; }
};
