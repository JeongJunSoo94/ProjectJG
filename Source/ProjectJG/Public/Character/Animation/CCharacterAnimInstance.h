#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CCharacterAnimInstance.generated.h"

UCLASS()
class PROJECTJG_API UCCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
protected:

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		float Speed;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		bool IsInAir;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		float Pitch;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		float Roll;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		float Yaw;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		FRotator RotationLastTick;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		float YawDelta;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		bool IsAccelerating;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		bool IsDie= false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		bool FullBody;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		bool bEquipped;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		float Direction;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		bool bAiming;
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	class ACharacter* OwnerCharacter;

};
