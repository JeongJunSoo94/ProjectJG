#include "Character/LtBelica/CLtBelicaWeapon.h"
#include "Global.h"
#include "Character/CBaseCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Bullet/CBullet.h"
#include "Animation/AnimMontage.h"
#include "particles/PxParticleSystem.h"
#include "Particles/ParticleSystem.h"
#include "Engine/StaticMeshActor.h"
#include "Sound/SoundCue.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "BaseSystem/ObjectPoolFactory.h"
#include "Character/Interface/Damageable.h"
#include "BaseSystem/InGameModeBase.h"
#include "BaseSystem/PoolObjectActorComponent.h"

UCLtBelicaWeapon::UCLtBelicaWeapon()
{
	PrimaryComponentTick.bCanEverTick = true;

	CHelpers::GetAsset<UAnimMontage>(&FireMontage, "AnimMontage'/Game/Developers/USER/Character/Fire_Montage.Fire_Montage'");

	CHelpers::GetAsset<UParticleSystem>(&FlashParticle, "ParticleSystem'/Game/ParagonLtBelica/FX/Particles/Belica/Abilities/Primary/FX/P_BelicaMuzzle.P_BelicaMuzzle'");
	CHelpers::GetAsset<UParticleSystem>(&EjectParticle, "ParticleSystem'/Game/ParagonLtBelica/FX/Particles/Belica/Abilities/Primary/FX/P_BelicaPrimary.P_BelicaPrimary'");

	CHelpers::GetAsset<USoundCue>(&FireSoundCue, "SoundCue'/Game/Developers/USER/Character/GunSoundCue/LtBelicaFire.LtBelicaFire'");

	CHelpers::GetClass<ACBullet>(&BulletClass, "Blueprint'/Game/Developers/USER/Bullet/BP_Cbullet.BP_Cbullet_C'");

	CHelpers::GetAsset<UParticleSystem>(&ImpactParticle, "ParticleSystem'/Game/ParagonLtBelica/FX/Particles/Belica/Abilities/Primary/FX/P_BelicaHitWorld.P_BelicaHitWorld'");

	CHelpers::GetAsset<UMaterialInstanceConstant>(&DecalMaterial, "MaterialInstanceConstant'/Game/Materials/M_Decal_Inst.M_Decal_Inst'");
	
}


void UCLtBelicaWeapon::OnHitPaticle(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	CheckNull(HitComponent);
	CheckNull(OtherActor);
	CheckNull(OtherComp);
	FRotator rotator = Hit.ImpactNormal.Rotation();
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, Hit.Location, rotator, true, EPSCPoolMethod::AutoRelease);
	UGameplayStatics::SpawnDecalAtLocation(GetWorld(), DecalMaterial, FVector(5), Hit.Location, rotator, 10.0f);
	IDamageable* character = Cast<IDamageable>(OtherActor);
	CheckNull(character);
	character->TakeDamage(UKismetMathLibrary::RandomIntegerInRange(10,20));

}

void UCLtBelicaWeapon::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter->bUseControllerRotationYaw = true;

	ObjectPoolFactory = CHelpers::GetComponent<UObjectPoolFactory>(GetWorld()->GetAuthGameMode());
	if (ObjectPoolFactory != nullptr)
	{
		ObjectPoolFactory->CreateObject(20, BulletClass);
	}
}


void UCLtBelicaWeapon::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCLtBelicaWeapon::SetOwnerCharacter(ACBaseCharacter* character)
{
	OwnerCharacter = character;
	MuzzleIndex = OwnerCharacter->GetMesh()->GetBoneIndex("SMG_Barrel");

}

void UCLtBelicaWeapon::Begin_Fire()
{
	//OwnerCharacter->bUseControllerRotationYaw = true;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	IsFiring = true;
	Firing();
}

void UCLtBelicaWeapon::Firing()
{
	CheckFalse(IsFiring);
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &UCLtBelicaWeapon::Firing, .1f, false);
	OwnerCharacter->PlayAnimMontage(FireMontage);

	FVector start, end, direction;
	OwnerCharacter->GetLocationAndDirection(start, end, direction,true,0.3f,0.4f);

	UGameplayStatics::SpawnEmitterAttached(FlashParticle, OwnerCharacter->GetMesh(), "SMG_Barrel", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true, EPSCPoolMethod::AutoRelease);
	//UGameplayStatics::SpawnEmitterAttached(EjectParticle, OwnerCharacter->GetMesh(), "SMG_Barrel", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset);

	FVector muzzleLocation = OwnerCharacter->GetMesh()->GetSocketLocation("SMG_Barrel");

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSoundCue, muzzleLocation);


	if (!!BulletClass)
	{
		ACBullet* bullet;
		bullet = Cast<ACBullet>(ObjectPoolFactory->SpawnObject(BulletClass));

		FTransform Transform = bullet->GetTransform();
		Transform.SetLocation(muzzleLocation);
		direction = UKismetMathLibrary::GetDirectionUnitVector(muzzleLocation,end);
		Transform.SetRotation(FQuat(direction.Rotation()));
		bullet->SetActorTransform(Transform);
		bullet->PoolObject->SetActorLifeTime(3.0f);
		if (!(bullet->bInitailized))
		{
			bullet->bInitailized = true;
			bullet->GetMesh()->OnComponentHit.AddDynamic(this, &UCLtBelicaWeapon::OnHitPaticle);
		}
		bullet->PoolObject->SetActive(true);

		//ACBullet* bullet= GetWorld()->SpawnActor<ACBullet>(BulletClass, muzzleLocation, direction.Rotation());
		//bullet->GetMesh()->OnComponentHit.AddDynamic(this, &UCLtBelicaWeapon::OnHitPaticle);
	}
	//direction = OwnerCharacter->GetMesh()->GetSocketTransform("SMG_Barrel").GetRotation();

}

void UCLtBelicaWeapon::End_Fire()
{
	//OwnerCharacter->bUseControllerRotationYaw = false;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
	IsFiring = false;
	OwnerCharacter->StopAnimMontage(FireMontage);
}

bool UCLtBelicaWeapon::GetIsFiring()
{
	return IsFiring;
}
