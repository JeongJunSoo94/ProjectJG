
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

	CHelpers::GetAsset<USoundCue>(&FireSoundCue, "SoundCue'/Game/Developers/GohyeongJu/Characters/Murdock/Sounds/laserGun_Cue.laserGun_Cue'");


}


void UMurdockUltimateSkillComponent::BeginUltimate()
{
	OwnerCharacter->PlayAnimMontage(UltimateAnim, 1.0f, "Default");
	OwnerCharacter->CameraLag(true, 7.0f);
	OwnerCharacter->MoveCamera(FVector(0, 60, 0),FVector(0,0,0),FVector(0,0,40));

	OwnerCharacter->StartCameraFOV(-30.0f, 1.0f, this, "EndZoomLag");
	
	IsStopSkill = false;
	currentLaserCount = maxLaserCount;
}

// [&ClassA]() {ClassA.SomeFunction();}
// TFunction<void()> FunctionToSave

void UMurdockUltimateSkillComponent::LoopUltimate()
{
	OwnerCharacter->StopAnimMontage(UltimateAnim);
	OwnerCharacter->PlayAnimMontage(UltimateLoopAnim);
}

void UMurdockUltimateSkillComponent::EndUltimate()
{
	OwnerCharacter->MoveCamera();
	OwnerCharacter->StartCameraFOV(30.0f, 1.0f,this,"EndZoomLag");
	//[&]() { UMurdockUltimateSkillComponent::EndZoomLag(); }
	OwnerCharacter->StopAnimMontage(UltimateLoopAnim);
	OwnerCharacter->PlayAnimMontage(UltimateAnim, 1.0f, "EndUltimateShot");
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

	if (currentLaserCount > 0 && OwnerCharacter->GetCurrentMontage() == UltimateLoopAnim)
	{
		FHitResult OutHit;

		FTransform muzzleTransform = OwnerCharacter->GetMesh()->GetSocketTransform("Muzzle");
		FVector muzzleLocation = muzzleTransform.GetLocation();
		FVector cameraOutStart, cameraOutEnd, cameraDirection;
		OwnerCharacter->GetLocationAndDirection(cameraOutStart, cameraOutEnd, cameraDirection);

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSoundCue, muzzleLocation);

		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this->GetOwner());

		
		//DrawDebugLine(GetWorld(), muzzleLocation, cameraOutEnd, FColor::Red, false, 1, 0, 1);

		//Clog::Log(OutHit.GetActor());

		//Clog::Log(muzzleTransform.GetRotation().Rotator());
		//Clog::Log(cameraDirection.Rotation());


		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), UltimateMuzzleShot, muzzleLocation, cameraDirection.Rotation(), true, EPSCPoolMethod::AutoRelease);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), UltimateLaserSight, muzzleLocation, cameraDirection.Rotation(), true, EPSCPoolMethod::AutoRelease);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), UltimateLaser, muzzleLocation, cameraDirection.Rotation(), true, EPSCPoolMethod::AutoRelease);
	
		OwnerCharacter->StopAnimMontage(UltimateLoopAnim);
		OwnerCharacter->PlayAnimMontage(UltimateAnim, 1.0f, "LoopOutUltimate");
	
		--currentLaserCount;

		//bullet->GetMesh()->OnComponentHit.AddDynamic(this, &UMurdockSpreadShotSkillComponent::OnHitPaticle);

		if (GetWorld()->LineTraceSingleByChannel(OutHit, muzzleLocation, cameraOutEnd, ECC_Visibility, CollisionParams))
		{
			FRotator ImpactDirection = (-OutHit.ImpactNormal).Rotation();

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), UltimateLaserImpact, OutHit.ImpactPoint + 12.0f*OutHit.ImpactNormal, ImpactDirection, true, EPSCPoolMethod::AutoRelease);
		}

	}
	
	if(currentLaserCount <= 0)
	{
		IsStopSkill = true;
	}

	//UGameplayStatics::SpawnEmitterAttached(UltimateMuzzleShot, Ownercharacter->GetMesh(), "Muzzle", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true, EPSCPoolMethod::AutoRelease);
	//UGameplayStatics::SpawnEmitterAttached(UltimateLaser, Ownercharacter->GetMesh(), "Muzzle", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true, EPSCPoolMethod::AutoRelease);
	//UGameplayStatics::SpawnEmitterAttached(UltimateLaserSight, Ownercharacter->GetMesh(), "Muzzle", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true, EPSCPoolMethod::AutoRelease);

}


void UMurdockUltimateSkillComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<AMurdock>(GetOwner());
}

void UMurdockUltimateSkillComponent::EndZoomLag()
{
	//Clog::Log("End CameraLag");
	OwnerCharacter->CameraLag(false, 7.0f);
}