

#include "Character/Murdock/MurdockSpreadShotSkillComponent.h"
#include "Global.h"
#include "Character/Murdock/Murdock.h"
#include "Character/CBaseCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Engine/StaticMeshActor.h"
#include "particles/PxParticleSystem.h"
#include "Particles/ParticleSystem.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"

//#include "BaseSystem/BasePooledObject.h"
#include "Sound/SoundCue.h"
#include "BaseSystem/ObjectPoolFactory.h"
#include "Character/Murdock/Bullets/MurdockSpreadShotBullet.h"
#include "Particles/Size/ParticleModuleSize.h"


UMurdockSpreadShotSkillComponent::UMurdockSpreadShotSkillComponent()
{
	CHelpers::GetAsset<UAnimMontage>(&SpreadShotAnim, "AnimMontage'/Game/Developers/GohyeongJu/Characters/Murdock/Montages/ShotGun_Montage.ShotGun_Montage'");
	
	CHelpers::GetAsset<UParticleSystem>(&SpreadShotFlash, "ParticleSystem'/Game/ParagonMurdock/FX/Particles/Abilities/SpreadShot/FX/P_Murdock_Spreadshot.P_Murdock_Spreadshot'");
	CHelpers::GetAsset<UParticleSystem>(&SpreadShotImpact, "ParticleSystem'/Game/ParagonMurdock/FX/Particles/Abilities/SpreadShot/FX/P_SpreadShotImpact.P_SpreadShotImpact'");

	CHelpers::GetClass<AMurdockSpreadShotBullet>(&BulletClass, "Blueprint'/Game/Developers/GohyeongJu/Characters/Murdock/Bullets/BP_MurdockSpreadShotBullet.BP_MurdockSpreadShotBullet_C'");

	CHelpers::GetAsset<USoundCue>(&FireSoundCue, "SoundCue'/Game/Developers/GohyeongJu/Characters/Murdock/Sounds/SpreadShot_Fire_Cue.SpreadShot_Fire_Cue'");
	CHelpers::GetAsset<USoundCue>(&ExplosionSoundCue, "SoundCue'/Game/Developers/GohyeongJu/Characters/Murdock/Sounds/SpreadShot_Explosion_Cue.SpreadShot_Explosion_Cue'");

	
	
}

void UMurdockSpreadShotSkillComponent::OnHitPaticle(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
	FRotator rotator = Hit.ImpactNormal.Rotation();
	float currentLifeTime = Cast<AMurdockSpreadShotBullet>(HitComponent->GetOwner())->GetHitTime();
	//Clog::Log(currentLifeTime);
	//UParticleSystemComponent* ParticleSystem = 
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSoundCue, Hit.Location);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SpreadShotImpact, Hit.Location, rotator,(FVector3d)((currentLifeTime*10.0f)), true, EPSCPoolMethod::AutoRelease);

	IDamageable* character = Cast<IDamageable>(OtherActor);
	CheckNull(character);
	character->TakeDamage(10.0f);


	//ParticleSystem->SetRelativeScale3D(ParticleSystem->GetRelativeScale3D() * currentLifeTime);

}

void UMurdockSpreadShotSkillComponent::CreateObjectPool()
{
	OwnerCharacter = Cast<AMurdock>(GetOwner());

	CHelpers::CreateActorComponent<UObjectPoolFactory>(OwnerCharacter, &ObjectPoolFactory, "SpreadShotSkillObjectFactory");
	ObjectPoolFactory->PoolSize = 20;

	ObjectPoolFactory->PooledObjectSubclass = BulletClass;
	ObjectPoolFactory->Initialized();
}


void UMurdockSpreadShotSkillComponent::BeginPlay()
{
	Super::BeginPlay();

	if (OwnerCharacter == Cast<AMurdock>(GetOwner()))
	{
		Clog::Log("SameOwner");

	}
	else
	{
		Clog::Log("OtherOwner");
		OwnerCharacter = Cast<AMurdock>(GetOwner());

	}

	//CreateObjectPool();
}


void UMurdockSpreadShotSkillComponent::ZoomInSpreadShot()
{
	
	OwnerCharacter->bUseControllerRotationYaw = true;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;

	OwnerCharacter->PlayAnimMontage(SpreadShotAnim);

	OwnerCharacter->StartCameraFOV(-30.0f,1.0f);
}

void UMurdockSpreadShotSkillComponent::LoopZoomMontage()
{

	
	OwnerCharacter->PlayAnimMontage(SpreadShotAnim,1.0f,"BeginLoop");
}

void UMurdockSpreadShotSkillComponent::ShootSpreadShot()
{
	OwnerCharacter->StartCameraFOV(30.0f, 1.0f);


	OwnerCharacter->bUseControllerRotationYaw = false;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;

	// >> Fire
	
	FVector start, end, direction;
	OwnerCharacter->GetLocationAndDirection(start, end, direction);
	FVector muzzleLocation = OwnerCharacter->GetMesh()->GetSocketLocation("Muzzle_03");

	UGameplayStatics::SpawnEmitterAttached(SpreadShotFlash, OwnerCharacter->GetMesh(), "Muzzle_03", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true, EPSCPoolMethod::AutoRelease);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSoundCue, muzzleLocation);
	if (!!BulletClass)
	{
		AMurdockSpreadShotBullet* bullet;
		bullet = Cast<AMurdockSpreadShotBullet>(ObjectPoolFactory->SpawnObject());

		//bullet->TeleportTo(muzzleLocation, direction.Rotation());
		FTransform Transform = bullet->GetTransform();
		Transform.SetLocation(muzzleLocation);
		Transform.SetRotation(FQuat(direction.Rotation()));
		bullet->SetActorTransform(Transform);
		bullet->SetActorLifeTime(3.0f);

		if (!(bullet->bInitailized))
		{
			bullet->bInitailized = true;
			bullet->GetMesh()->OnComponentHit.AddDynamic(this, &UMurdockSpreadShotSkillComponent::OnHitPaticle);
		}
		bullet->SetActive(true);
	}


	
	// << Fire

	OwnerCharacter->PlayAnimMontage(SpreadShotAnim,1.0f,"EndLoop");
	
}