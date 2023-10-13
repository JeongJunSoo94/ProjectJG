#include "Character/Animation/CCharacterAnimInstance.h"
#include "Global.h"
#include "GameFramework/Character.h"

void UCCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());

}

void UCCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	CheckNull(OwnerCharacter);

	Speed = OwnerCharacter->GetVelocity().Size2D();
	if (Speed > 0)
		IsAccelerating = true;
	else
		IsAccelerating = false;

	Direction = CalculateDirection(OwnerCharacter->GetVelocity(), OwnerCharacter->GetControlRotation());

	Pitch = OwnerCharacter->GetBaseAimRotation().Pitch;

}
