#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "WorldObjects/Item/WeaponType.h"
#include "CCharacterAnimInstance.generated.h"

UENUM(BlueprintType)
enum class EOffsetState : uint8
{
	EOS_Aiming UMETA(DisplayName = "Aiming"),
	EOS_Hip UMETA(DisplayName = "Hip"),
	EOS_Reloading UMETA(DisplayName = "Reloading"),
	EOS_InAir UMETA(DisplayName = "InAir"),


	EOS_MAX UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class PROJECTJG_API UCCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UCCharacterAnimInstance();

	//UFUNCTION(BlueprintCallable)
	//	void UpdateAnimationProperties(float DeltaTime);

	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
protected:
	void TurnInPlace();

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

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		FRotator RotationLastTick;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Lean, meta = (AllowPrivateAccess = "true"))
		double YawDelta;

	float TIPCharacterYaw;
	float TIPCharacterYawLastFrame;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn In Place", meta = (AllowPrivateAccess = "true"))
		float RootYawOffset;

	float RotationCurve;
	float RotationCurveLastFrame;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn In Place", meta = (AllowPrivateAccess = "true"))
		bool bReloading;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn In Place", meta = (AllowPrivateAccess = "true"))
		EOffsetState OffsetState;

	FRotator CharacterRotation;

	FRotator CharacterRotationLastFrame;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Crouching, meta = (AllowPrivateAccess = "true"))
		bool bCrouching;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Crouching, meta = (AllowPrivateAccess = "true"))
		bool bEquipping;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		float RecoilWeight;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		bool bTurningInPlace;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		EWeaponType EquippedWeaponType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		bool bShouldUseFABRIK;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		bool bFullBody;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		bool bIsDie= false;
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
};
