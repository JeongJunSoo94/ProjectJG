// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Character/TurningInPlace.h"
#include "Character/Animation/CCharacterAnimInstance.h"
#include "CharacterAnimInstance.generated.h"

UCLASS()
class PROJECTJG_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UCharacterAnimInstance();
	UFUNCTION(BlueprintCallable)
		void UpdateAnimationProperties(float DeltaTime);

	void NativeInitializeAnimation();
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	void TurnInPlace();
	void CalculateAO_Pitch();
	void Lean(float DeltaTime);

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float Speed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		bool bIsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float MovementOffsetYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float LastMovementOffsetYaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		bool bIsAccelerating;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		bool bAiming;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		bool bUseAimOffsets;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		FRotator RotationLastTick;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Lean, meta = (AllowPrivateAccess = "true"))
		double YawDelta;

	//float TIPCharacterYaw;
	//float TIPCharacterYawLastFrame;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float RootYawOffset;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float AO_Yaw;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float AO_Pitch;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
		FTransform LeftHandTransform;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
		FTransform RightHandTransform;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
		FRotator LeftHandRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
		FRotator RightHandRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
		bool bLeftHandTransform;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
		bool bRightHandTransform;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
		ETurningInPlace TurningInPlace;
	
	float RotationCurve;
	float RotationCurveLastFrame;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn In Place", meta = (AllowPrivateAccess = "true"))
		bool bRotateRootBone;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn In Place", meta = (AllowPrivateAccess = "true"))
		bool bReloading;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn In Place", meta = (AllowPrivateAccess = "true"))
		EOffsetState OffsetState;

	FRotator CharacterRotator;

	FRotator CharacterRotatorLastFrame;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Crouching, meta = (AllowPrivateAccess = "true"))
		bool bCrouching;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Crouching, meta = (AllowPrivateAccess = "true"))
		bool bEquipping;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		float RecoilWeight;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	//	bool bTurningInPlace;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		EWeaponType EquippedWeaponType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		bool bShouldUseFABRIK;

	FRotator HitTargetAtLookRot;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class ABaseCharacter* OwnerCharacter;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		bool bTransformRightHand;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		bool bLocallyControlled;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		bool bFullBody;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		bool bElimmed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		bool bEquipped;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		float Direction;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		float Pitch;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		float Roll;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		float Yaw;

	FString CharacterInfoStr;
public:
	FORCEINLINE FString GetCharacterInfo() const { return CharacterInfoStr; }
};