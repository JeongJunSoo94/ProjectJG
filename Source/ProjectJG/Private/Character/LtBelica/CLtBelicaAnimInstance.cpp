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
}