#include "Character/Animation/CCharacterAnimInstance.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UCCharacterAnimInstance::UCCharacterAnimInstance()
{
}

//void UCCharacterAnimInstance::UpdateAnimationProperties(float DeltaTime)
//{
//}

void UCCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());

	bIsInAir = false;

}

void UCCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	CheckNull(OwnerCharacter);

	Speed = OwnerCharacter->GetVelocity().Size2D();
	if (Speed > 0)
		bIsAccelerating = true;
	else
		bIsAccelerating = false;

	Direction = CalculateDirection(OwnerCharacter->GetVelocity(), OwnerCharacter->GetControlRotation());
	Pitch = OwnerCharacter->GetBaseAimRotation().Pitch;

	bIsInAir = OwnerCharacter->GetMovementComponent()->IsFalling();
}

void UCCharacterAnimInstance::TurnInPlace()
{
}

void UCCharacterAnimInstance::Lean(float DeltaTime)
{
	if (OwnerCharacter == nullptr) return;
	CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation = OwnerCharacter->GetActorRotation();

	const FRotator Delta{ UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame) };

	const double Target{ Delta.Yaw / DeltaTime };
	const double Interp{ FMath::FInterpTo(YawDelta, Target, DeltaTime, 6.f) };
	YawDelta = FMath::Clamp(Interp, -90.f, 90.f);
}