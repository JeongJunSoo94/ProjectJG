#include "Character/Enemies/MeleeEnemy/MeleeCharacterAnimInstance.h"
#include "Global.h"
#include "Character/Enemies/MeleeEnemy/MeleeEnemyCharacter.h"

void UMeleeCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();


	MeleeCharacter = Cast<AMeleeEnemyCharacter>(TryGetPawnOwner());
}

void UMeleeCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	CheckNull(MeleeCharacter);

	//IsFiring = MeleeCharacter->GetLtBelicaWeaponIsFiring();

	//IsQAbiliting = LtBelicaCharacter->GetLtBelicaIsAbiliting();

	bFullBody = MeleeCharacter->IsFullBody;
	bIsDie = MeleeCharacter->IsDie;
}
