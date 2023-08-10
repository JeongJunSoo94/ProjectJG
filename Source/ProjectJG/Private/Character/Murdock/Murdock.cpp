
#include "Character/Murdock/Murdock.h"
#include "Global.h"
#include "Character/CBaseCharacter.h"
#include "Character/Murdock/MurdockWeapon.h"
#include "Camera/CameraComponent.h"
#include "Character/LtBelica/CQAbliltyActionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Character/Murdock/MurdockShieldSkillComponent.h"
#include "Character/Murdock/MurdockTazerTrapSkillComponent.h"
#include "Character/Murdock/MurdockSpreadShotSkillComponent.h"
#include "Character/Murdock/MurdockUltimateSkillComponent.h"

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


	CHelpers::CreateActorComponent<UMurdockUltimateSkillComponent>(this, &UltimateSkill, "ElevenSkill");
	CHelpers::CreateActorComponent<UMurdockShieldSkillComponent>(this, &ShieldSkill, "ShieldSkill");
	CHelpers::CreateActorComponent<UMurdockSpreadShotSkillComponent>(this, &SpreadShotSkill, "SpreadShotSkill");
	CHelpers::CreateActorComponent<UMurdockTazerTrapSkillComponent>(this, &TazerTrapSkill, "TazerTrapSkill");

	SpreadShotSkill->CreateObjectPool();
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
	PlayerInputComponent->BindAction("ForwardLook_Move", EInputEvent::IE_Pressed, this, &AMurdock::OnAlt);
	PlayerInputComponent->BindAction("ForwardLook_Move", EInputEvent::IE_Released, this, &AMurdock::OffAlt);
	PlayerInputComponent->BindAction("Q", EInputEvent::IE_Pressed, this, &AMurdock::OnShield);
	PlayerInputComponent->BindAction("Q", EInputEvent::IE_Released, this, &AMurdock::OffShield);
	PlayerInputComponent->BindAction("E", EInputEvent::IE_Pressed, this, &AMurdock::OnSpreadShot);
	PlayerInputComponent->BindAction("E", EInputEvent::IE_Released, this, &AMurdock::OffSpreadShot);
	PlayerInputComponent->BindAction("R", EInputEvent::IE_Pressed, this, &AMurdock::OnUltimate);
	PlayerInputComponent->BindAction("R", EInputEvent::IE_Released, this, &AMurdock::OffUltimate);
}

void AMurdock::OnFire()
{
	if (BehaviorState == MurdockBehaviorState::EIdle)
	{
		MurdockWeapon->Begin_Fire();
		BehaviorState = MurdockBehaviorState::EFire;
	}

}

void AMurdock::OffFire()
{
	MurdockWeapon->End_Fire();
	BehaviorState = MurdockBehaviorState::EIdle;
}

void AMurdock::OnAlt()
{
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
}
void AMurdock::OffAlt()
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AMurdock::OnShield()
{
	if (BehaviorState == MurdockBehaviorState::EIdle)
	{
		ShieldSkill->BeginShield();
		BehaviorState = MurdockBehaviorState::EShield;
	}
}
void AMurdock::OffShield()
{
	ShieldSkill->BreakShield();
	BehaviorState = MurdockBehaviorState::EIdle;
}
void AMurdock::LoopShield()
{
	ShieldSkill->LoopShieldMontage();
}

void AMurdock::OnSpreadShot()
{
	if (BehaviorState == MurdockBehaviorState::EIdle)
	{
		SpreadShotSkill->ZoomInSpreadShot();
		BehaviorState = MurdockBehaviorState::ESpreadShot;
	}
}
void AMurdock::OffSpreadShot()
{
	SpreadShotSkill->ShootSpreadShot();
	BehaviorState = MurdockBehaviorState::EIdle;
}

void AMurdock::LoopSpreadShotZoom()
{
	SpreadShotSkill->LoopZoomMontage();
}

void AMurdock::LoopUltimate()
{
	if (BehaviorState == MurdockBehaviorState::EUltimate)
	{
		UltimateSkill->LoopUltimate();
		Stop();
	}
}

void AMurdock::OnUltimate()
{

	if (BehaviorState == MurdockBehaviorState::EIdle)
	{
		//Clog::Log(int(BehaviorState));
		FVector FrontVector = UKismetMathLibrary::Cross_VectorVector(PlayerMainCamera->GetRightVector(), GetActorUpVector());
		SetActorRotation(FrontVector.Rotation().Quaternion());
	
		GetCharacterMovement()->bOrientRotationToMovement = false;
		FrontYaw = GetBaseAimRotation().Yaw;
		
		UltimateSkill->BeginUltimate();
		BehaviorState = MurdockBehaviorState::EUltimate;
		
	}
}
void AMurdock::OffUltimate()
{
	if (BehaviorState == MurdockBehaviorState::EUltimate)
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
		UltimateSkill->EndUltimate();
		SolveStop();
	}
}

void AMurdock::EndUltimateToIdle()
{
	BehaviorState = MurdockBehaviorState::EIdle;
}