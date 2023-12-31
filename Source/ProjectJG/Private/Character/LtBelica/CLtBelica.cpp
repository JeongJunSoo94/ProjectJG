#include "Character/LtBelica/CLtBelica.h"
#include "Global.h"
#include "Character/LtBelica/CLtBelicaWeapon.h"
#include "Camera/CameraComponent.h"
#include "Character/LtBelica/CQAbliltyActionComponent.h"

ACLtBelica::ACLtBelica()
{
	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/ParagonLtBelica/Characters/Heroes/Belica/Meshes/Belica.Belica'");
	GetMesh()->SetSkeletalMesh(mesh);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	weaponBoneIdexs.Add(GetMesh()->GetBoneIndex("weapon"));
	CHelpers::CreateActorComponent<UCLtBelicaWeapon>(this, &LtBelicaWeapon,"LtBelicaWeapon");
	LtBelicaWeapon->SetOwnerCharacter(this);

	CHelpers::CreateActorComponent<UCQAbliltyActionComponent>(this, &LtBelicaQAbility, "LtBelicaQAbility");
	LtBelicaQAbility->SetOwnerCharacter(this);

}

void ACLtBelica::BeginPlay()
{
	Super::BeginPlay();

}

void ACLtBelica::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ACLtBelica::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &ACLtBelica::OnFire);
	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Released, this, &ACLtBelica::OffFire);

	PlayerInputComponent->BindAction("Q", EInputEvent::IE_Pressed, this, &ACLtBelica::OnQAbility);
}

void ACLtBelica::OnFire()
{
	LtBelicaWeapon->Begin_Fire();
}

void ACLtBelica::OffFire()
{
	LtBelicaWeapon->End_Fire();
}

void ACLtBelica::OnQAbility()
{
	LtBelicaQAbility->OnAction();
}

bool ACLtBelica::GetLtBelicaWeaponIsFiring()
{
	return LtBelicaWeapon->GetIsFiring();
}

bool ACLtBelica::GetLtBelicaIsAbiliting()
{
	return LtBelicaQAbility->GetIsAbiliting();
}