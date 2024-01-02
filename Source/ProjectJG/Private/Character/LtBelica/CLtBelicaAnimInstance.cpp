#include "Character/LtBelica/CLtBelicaAnimInstance.h"
#include "Global.h"
#include "Character/LtBelica/CLtBelica.h"

void UCLtBelicaAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();


	LtBelicaCharacter = Cast<ACLtBelica>(TryGetPawnOwner());
}

void UCLtBelicaAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	CheckNull(LtBelicaCharacter);

	IsFiring = LtBelicaCharacter->GetLtBelicaWeaponIsFiring();

	IsQAbiliting = LtBelicaCharacter->GetLtBelicaIsAbiliting();

	/*if (OwnerCharacter->GetBaseAimRotation().Pitch > 0)
		Pitch = OwnerCharacter->GetBaseAimRotation().Pitch;
	else if (OwnerCharacter->GetBaseAimRotation().Pitch >= -56)
		Pitch = OwnerCharacter->GetBaseAimRotation().Pitch * 1.6;*/
	//
	if (LtBelicaCharacter->GetAbilityState() == EBelicaAbilityState::Fire)
	{
		Clog::Log(WeaponRotation);
		if (OwnerCharacter->GetBaseAimRotation().Pitch > 0)
			Pitch = OwnerCharacter->GetBaseAimRotation().Pitch;
		else if (OwnerCharacter->GetBaseAimRotation().Pitch >= -56)
			Pitch = OwnerCharacter->GetBaseAimRotation().Pitch * 1.6;

		Yaw = LtBelicaCharacter->SetFocusActorYawDegree();
	}

	//Yaw = LtBelicaCharacter->SetFocusActorYawDegree();
}