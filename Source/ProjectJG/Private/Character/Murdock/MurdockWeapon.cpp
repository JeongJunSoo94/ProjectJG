
#include "Character/Murdock/MurdockWeapon.h"
#include "Global.h"
#include "Character/CBaseCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Bullet/CBullet.h"
#include "Character/Murdock/MurdockBullet.h"
#include "Animation/AnimMontage.h"
#include "particles/PxParticleSystem.h"
#include "Particles/ParticleSystem.h"
#include "Engine/StaticMeshActor.h"
#include "Sound/SoundCue.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "BaseSystem/ObjectPoolFactory.h"
#include "BaseSystem/InGameModeBase.h"
#include "BaseSystem/PoolObjectActorComponent.h"

UMurdockWeapon::UMurdockWeapon()
{

	CHelpers::GetAsset<UAnimMontage>(&FireMontage, "AnimMontage'/Game/Developers/GohyeongJu/Characters/Murdock/Montages/Fire_ShotGun_Montage.Fire_ShotGun_Montage'");

	CHelpers::GetAsset<UParticleSystem>(&FlashParticle, "ParticleSystem'/Game/ParagonMurdock/FX/Particles/Abilities/Primary/FX/P_Murdock_STD_MUZZLE.P_Murdock_STD_MUZZLE'");
	CHelpers::GetAsset<UParticleSystem>(&EjectParticle, "ParticleSystem'/Game/ParagonMurdock/FX/Particles/Abilities/Primary/FX/P_Murdock_STD_MUZZLE_Glow.P_Murdock_STD_MUZZLE_Glow'");

	CHelpers::GetAsset<USoundCue>(&FireSoundCue, "SoundCue'/Game/Developers/GohyeongJu/Characters/Murdock/Sounds/Murdock_primary_shot.Murdock_primary_shot'");

	CHelpers::GetClass<AMurdockBullet>(&BulletClass, "Blueprint'/Game/Developers/GohyeongJu/Characters/Murdock/Bullets/BP_MurdockBullet.BP_MurdockBullet_C'");

	CHelpers::GetAsset<UParticleSystem>(&ImpactParticle, "ParticleSystem'/Game/ParagonMurdock/FX/Particles/Abilities/Primary/FX/P_Murdock_STD_MUZZLE_Glow.P_Murdock_STD_MUZZLE_Glow'");

	CHelpers::GetAsset<UMaterialInstanceConstant>(&DecalMaterial, "MaterialInstanceConstant'/Game/Materials/M_Decal_Inst.M_Decal_Inst'");
	
	
}


void UMurdockWeapon::OnHitPaticle(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	/*Clog::Log("---OnHitParticle---");
	CheckNull(HitComponent);
	Clog::Log(HitComponent->GetOwner());
	CheckNull(OtherActor);
	Clog::Log(OtherActor->GetName());
	CheckNull(OtherComp);
	Clog::Log(OtherComp->GetOwner());
	Clog::Log("---OnHitParticle---");*/

	

	FRotator rotator = Hit.ImpactNormal.Rotation();
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, Hit.Location, rotator, true, EPSCPoolMethod::AutoRelease);
	UGameplayStatics::SpawnDecalAtLocation(GetWorld(), DecalMaterial, FVector(5), Hit.Location, rotator, 10.0f);

	IDamageable* character = Cast<IDamageable>(OtherActor);
	CheckNull(character);
	character->TakeDamage(1.0f);
	character->BeginHitEffect(OwnerCharacter, NormalImpulse, Hit);
	
}

void UMurdockWeapon::BeginPlay()
{
	Super::BeginPlay();
	ObjectPoolFactory = CHelpers::GetComponent<UObjectPoolFactory>(GetWorld()->GetAuthGameMode());
	if (ObjectPoolFactory != nullptr)
	{
		ObjectPoolFactory->CreateObject(5, BulletClass);
	}

}


void UMurdockWeapon::SetOwnerCharacter(ACBaseCharacter* character)
{
	OwnerCharacter = Cast<ACBaseCharacter>(GetOwner());
	MuzzleIndex = OwnerCharacter->GetMesh()->GetBoneIndex("Muzzle");
	//CHelpers::CreateActorComponent<UObjectPoolFactory>(OwnerCharacter, &ObjectPoolFactory, "ObjectPoolFactory");
	//ObjectPoolFactory->PoolSize = 20;
	//
	//ObjectPoolFactory->PooledObjectSubclass = BulletClass;
	//ObjectPoolFactory->Initialized();
	OwnerCharacter->bUseControllerRotationYaw = true;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	
}


void UMurdockWeapon::Begin_Fire()
{
	CheckTrue(IsFiring);

	OwnerCharacter->bUseControllerRotationYaw = true;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	IsFiring = true;
	Firing();
}

void UMurdockWeapon::Firing()
{
	CheckFalse(IsFiring);
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &UMurdockWeapon::Firing, .1f, false);

	OwnerCharacter->PlayAnimMontage(FireMontage);

	FVector start, end, direction;
	OwnerCharacter->GetLocationAndDirection(start, end, direction);
	FRotator rotator_90(0, 0, -90);

	UGameplayStatics::SpawnEmitterAttached(FlashParticle, OwnerCharacter->GetMesh(), "Muzzle", FVector::ZeroVector, rotator_90 , EAttachLocation::KeepRelativeOffset, true, EPSCPoolMethod::AutoRelease);
	UGameplayStatics::SpawnEmitterAttached(EjectParticle, OwnerCharacter->GetMesh(), "Muzzle", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true, EPSCPoolMethod::AutoRelease);

	
	FVector muzzleLocation = OwnerCharacter->GetMesh()->GetSocketLocation("Muzzle");
	//muzzleLocation += 400 * direction;
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSoundCue, muzzleLocation );


	if (!!BulletClass)
	{
		AMurdockBullet* bullet;
		bullet = Cast<AMurdockBullet>(ObjectPoolFactory->SpawnObject(BulletClass));
	
		//bullet->TeleportTo(muzzleLocation, direction.Rotation());
		FTransform Transform = bullet->GetTransform();
		Transform.SetLocation(muzzleLocation);
		Transform.SetRotation(FQuat(direction.Rotation()));
		bullet->SetActorTransform(Transform);
		bullet->PoolObject->SetActorLifeTime(3.0f);
		
		if (!(bullet->bInitailized))
		{
			//Clog::Log("Bind");
			bullet->bInitailized = true;
			bullet->GetMesh()->OnComponentHit.AddDynamic(this, &UMurdockWeapon::OnHitPaticle);
		}
		bullet->PoolObject->SetActive(true);

		//ACBullet* bullet= GetWorld()->SpawnActor<ACBullet>(BulletClass, muzzleLocation, direction.Rotation());
		//bullet->GetMesh()->OnComponentHit.AddDynamic(this, &UCLtBelicaWeapon::OnHitPaticle);
	}
	//direction = OwnerCharacter->GetMesh()->GetSocketTransform("SMG_Barrel").GetRotation();

}

void UMurdockWeapon::End_Fire()
{
	OwnerCharacter->bUseControllerRotationYaw = false;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
	IsFiring = false;
	OwnerCharacter->StopAnimMontage(FireMontage);
}

bool UMurdockWeapon::GetIsFiring()
{
	return IsFiring;
}

void UMurdockWeapon::OnStartAction()
{
	Begin_Fire();
}

void UMurdockWeapon::OnEndAction()
{
	End_Fire();
}
