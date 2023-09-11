// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CBaseCharacter.h"
#include "Character/Interface/ActionNotifiable.h"
#include "Character/Components/CActionComponent.h"
#include "Murdock.generated.h"

DECLARE_DYNAMIC_DELEGATE(FZoomDelegate);

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
class PROJECTJG_API AMurdock : public ACBaseCharacter , public IActionNotifiable
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

protected:
	UPROPERTY(VisibleDefaultsOnly)
		class UCurveFloat* ZoomCurveFloat;


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

	float increaseFOV;
	float currentFOV;
	bool isChangeFOV = false;
	float duringTime = 0.0f;
	float reverseDuringTime = 0.0f;
	float ZoomTime = 0.0f;
	void DoZoom(float DeltaTime);


public:
	FZoomDelegate EndZoomFunc;

	FORCEINLINE MurdockBehaviorState GetBehaviorState() { return BehaviorState; }
	FORCEINLINE void SetBehaviorState(MurdockBehaviorState State) { BehaviorState = State; }

	void CameraLag(bool Active, float CameraLagSpeed);
	template<typename T>
	void StartCameraFOV(float IncreaseFOV, float DuringTime, T* OtherClass ,const FName FunctionName);
	void StartCameraFOV(float IncreaseFOV, float DuringTime);

	void MoveCamera(FVector socketOffset = FVector(0, 60, 0), FVector tragetOffset = FVector::ZeroVector, FVector location = FVector(0, 0, 60), FRotator rotation = FRotator::ZeroRotator, float targetArmLength = 200.0f);
	void MoveCamera(FName targetName);

	float GetFrontYaw() { return FrontYaw; }
	UCActionComponent* GetActionComponent() ;

	void OnStartActionMontage(MurdockBehaviorState ActionEnum);
	void OnEndActionMontage(MurdockBehaviorState ActionEnum = MurdockBehaviorState::EIdle);

	void BeginNotifyAction();
	void MiddleNotifyAction() ;
	void EndNotifyAction() ;
};
