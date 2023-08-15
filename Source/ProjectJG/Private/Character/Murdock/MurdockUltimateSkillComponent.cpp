
#include "Character/Murdock/MurdockUltimateSkillComponent.h"
#include "Global.h"
#include "Character/CBaseCharacter.h"
#include "Character/Murdock/Murdock.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Engine/StaticMeshActor.h"
#include "particles/PxParticleSystem.h"
#include "Particles/ParticleSystem.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"

UMurdockUltimateSkillComponent::UMurdockUltimateSkillComponent()
{
	CHelpers::GetAsset<UAnimMontage>(&UltimateAnim, "AnimMontage'/Game/Developers/GohyeongJu/Characters/Murdock/Montages/Ultimate_Montage.Ultimate_Montage'");
	CHelpers::GetAsset<UAnimMontage>(&UltimateLoopAnim, "AnimMontage'/Game/Developers/GohyeongJu/Characters/Murdock/Montages/Ultimate_Loop_Montage.Ultimate_Loop_Montage'");

	CHelpers::GetAsset<UParticleSystem>(&UltimateLoop, "ParticleSystem'/Game/ParagonMurdock/FX/Particles/Abilities/Ultimate/FX/P_Eleven_GunDetails_ChargeUp.P_Eleven_GunDetails_ChargeUp'");
	CHelpers::GetAsset<UParticleSystem>(&UltimateBeginShot, "ParticleSystem'/Game/ParagonMurdock/FX/Particles/Abilities/Ultimate/FX/P_Eleven_GunDetails_PowerCanister.P_Eleven_GunDetails_PowerCanister'");
	CHelpers::GetAsset<UParticleSystem>(&UltimateLaser, "ParticleSystem'/Game/ParagonMurdock/FX/Particles/Abilities/Ultimate/FX/P_ElevenBeam_NotSmoke.P_ElevenBeam_NotSmoke'");
	CHelpers::GetAsset<UParticleSystem>(&UltimateLaserSight, "ParticleSystem'/Game/ParagonMurdock/FX/Particles/Abilities/Ultimate/FX/P_Eleven_LaserSight.P_Eleven_LaserSight'");
	CHelpers::GetAsset<UParticleSystem>(&UltimateMuzzleShot, "ParticleSystem'/Game/ParagonMurdock/FX/Particles/Abilities/Ultimate/FX/P_Eleven_Muzzle.P_Eleven_Muzzle'");
	CHelpers::GetAsset<UParticleSystem>(&UltimateLaserImpact, "ParticleSystem'/Game/ParagonMurdock/FX/Particles/Abilities/Ultimate/FX/P_Eleven_Impact_Player.P_Eleven_Impact_Player'");
}

void UMurdockUltimateSkillComponent::BeginUltimate()
{
	OwnerCharacter->PlayAnimMontage(UltimateAnim, 1.0f, "Default");
	OwnerCharacter->CameraLag(true, 7.0f);
	OwnerCharacter->MoveCamera(FVector(140.0f, 0.0f, -20.0f));


}

void UMurdockUltimateSkillComponent::LoopUltimate()
{
	OwnerCharacter->StopAnimMontage(UltimateAnim);
	OwnerCharacter->PlayAnimMontage(UltimateLoopAnim);
}

void UMurdockUltimateSkillComponent::EndUltimate()
{
	OwnerCharacter->CameraLag(false, 10.0f);
	OwnerCharacter->MoveCamera();

	OwnerCharacter->StopAnimMontage(UltimateLoopAnim);
	OwnerCharacter->PlayAnimMontage(UltimateAnim,1.0f,"LoopOutUltimate");
}

void UMurdockUltimateSkillComponent::ChargeLaser()
{
	UGameplayStatics::SpawnEmitterAttached(UltimateLoop, OwnerCharacter->GetMesh(), "Muzzle", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true, EPSCPoolMethod::AutoRelease);
	UGameplayStatics::SpawnEmitterAttached(UltimateBeginShot, OwnerCharacter->GetMesh(), "Muzzle", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true, EPSCPoolMethod::AutoRelease);


}

void UMurdockUltimateSkillComponent::ShotLaser()
{
	//Ownercharacter->GetMesh()->get
	//Ownercharacter->GetMesh()->GetSocketTransform("Muzzle");
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), UltimateMuzzleShot, OwnerCharacter->GetMesh()->GetSocketTransform("Muzzle"), true, EPSCPoolMethod::AutoRelease);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), UltimateLaser, OwnerCharacter->GetMesh()->GetSocketTransform("Muzzle"), true, EPSCPoolMethod::AutoRelease);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), UltimateLaserSight, OwnerCharacter->GetMesh()->GetSocketTransform("Muzzle"), true, EPSCPoolMethod::AutoRelease);

	
	//UGameplayStatics::SpawnEmitterAttached(UltimateMuzzleShot, Ownercharacter->GetMesh(), "Muzzle", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true, EPSCPoolMethod::AutoRelease);
	//UGameplayStatics::SpawnEmitterAttached(UltimateLaser, Ownercharacter->GetMesh(), "Muzzle", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true, EPSCPoolMethod::AutoRelease);
	//UGameplayStatics::SpawnEmitterAttached(UltimateLaserSight, Ownercharacter->GetMesh(), "Muzzle", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true, EPSCPoolMethod::AutoRelease);

}


void UMurdockUltimateSkillComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<AMurdock>(GetOwner());
}
