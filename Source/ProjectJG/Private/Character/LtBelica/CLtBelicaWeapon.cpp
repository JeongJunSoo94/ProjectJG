#include "Character/LtBelica/CLtBelicaWeapon.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Animation/AnimMontage.h"

UCLtBelicaWeapon::UCLtBelicaWeapon()
{
	PrimaryComponentTick.bCanEverTick = true;

	CHelpers::GetAsset<UAnimMontage>(&FireMontage, "AnimMontage'/Game/Developers/USER/Character/Fire_Montage.Fire_Montage'");
}

void UCLtBelicaWeapon::BeginPlay()
{
	Super::BeginPlay();

}


void UCLtBelicaWeapon::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCLtBelicaWeapon::SetOwnerCharacter(ACharacter* character)
{
	OwnerCharacter = character;
}

void UCLtBelicaWeapon::Begin_Fire()
{
	Firing();
}

void UCLtBelicaWeapon::Firing()
{
	OwnerCharacter->PlayAnimMontage(FireMontage);
}

void UCLtBelicaWeapon::End_Fire()
{
	OwnerCharacter->StopAnimMontage(FireMontage);
}

