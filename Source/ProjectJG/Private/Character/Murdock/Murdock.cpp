
#include "Character/Murdock/Murdock.h"
#include "Global.h"
#include "Character/Murdock/MurdockWeapon.h"
#include "Camera/CameraComponent.h"
#include "Character/LtBelica/CQAbliltyActionComponent.h"

AMurdock::AMurdock()
{
	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/ParagonMurdock/Characters/Heroes/Murdock/Meshes/Murdock.Murdock'");
	GetMesh()->SetSkeletalMesh(mesh);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	weaponBoneIdexs.Add(GetMesh()->GetBoneIndex("weapon"));
	CHelpers::CreateActorComponent<UMurdockWeapon>(this, &MurdockWeapon, "MurdockWeapon");
	MurdockWeapon->SetOwnerCharacter(this);

}

void AMurdock::BeginPlay()
{
	Super::BeginPlay();

}

void AMurdock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AMurdock::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &AMurdock::OnFire);
	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Released, this, &AMurdock::OffFire);
}

void AMurdock::OnFire()
{
	MurdockWeapon->Begin_Fire();
}

void AMurdock::OffFire()
{
	MurdockWeapon->End_Fire();
}

void AMurdock::EndShotGunFireAnim()
{
	OffFire();
}

