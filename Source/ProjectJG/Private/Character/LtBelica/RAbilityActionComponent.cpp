#include "Character/LtBelica/RAbilityActionComponent.h"
#include "Global.h"
#include "Character/CBaseCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "particles/PxParticleSystem.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Sound/SoundCue.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "BaseSystem/ObjectPoolFactory.h"
#include "Character/LtBelica/EruptionHologram.h"
#include "Character/LtBelica/EruptionObject.h"
#include "Bullet/CBullet.h"
#include "Sound/SoundCue.h"
#include "BaseSystem/InGameModeBase.h"
#include "BaseSystem/PoolObjectActorComponent.h"

void URAbilityActionComponent::OnHitPaticle(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Clog::Log("hit");
	FRotator rotator = Hit.ImpactNormal.Rotation();
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, Hit.Location, rotator, true, EPSCPoolMethod::AutoRelease);
	UGameplayStatics::SpawnDecalAtLocation(GetWorld(), DecalMaterial, FVector(5), Hit.Location, rotator, 10.0f);
}

URAbilityActionComponent::URAbilityActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	CHelpers::GetAsset<UAnimMontage>(&RAbliltyMontage, "AnimMontage'/Game/Developers/JJS/LtBelica/Animation/R_Ability_Montage.R_Ability_Montage'");

	//CHelpers::GetAsset<UParticleSystem>(&FlashParticle, "ParticleSystem'/Game/ParagonLtBelica/FX/Particles/Belica/Abilities/Primary/FX/P_BelicaMuzzle.P_BelicaMuzzle'");
	//CHelpers::GetAsset<UParticleSystem>(&EjectParticle, "ParticleSystem'/Game/ParagonLtBelica/FX/Particles/Belica/Abilities/Primary/FX/P_BelicaPrimary.P_BelicaPrimary'");

	//CHelpers::GetAsset<USoundCue>(&FireSoundCue, "SoundCue'/Game/Developers/USER/Character/GunSoundCue/LtBelicaFire.LtBelicaFire'");

	CHelpers::GetClass<ACBullet>(&BulletClass, "Blueprint'/Game/Developers/USER/Bullet/BP_Cbullet.BP_Cbullet_C'");

	CHelpers::GetAsset<UParticleSystem>(&ImpactParticle, "ParticleSystem'/Game/ParagonLtBelica/FX/Particles/Belica/Abilities/Primary/FX/P_BelicaHitWorld.P_BelicaHitWorld'");

	CHelpers::GetAsset<UMaterialInstanceConstant>(&DecalMaterial, "MaterialInstanceConstant'/Game/Materials/M_Decal_Inst.M_Decal_Inst'");

}

void URAbilityActionComponent::BeginPlay()
{
	IsCoolTiming = false;
	IntervalCoolTime = 0.1f;
	CurCoolTime = 0;
	MaxCoolTime = 10.0f;
	ObjectPoolFactory = CHelpers::GetComponent<UObjectPoolFactory>(GetWorld()->GetAuthGameMode());
	if (ObjectPoolFactory != nullptr)
	{
		ObjectPoolFactory->CreateObject(5, BulletClass);
	}
}

void URAbilityActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	CheckFalse(IsAbiliting);

}

void URAbilityActionComponent::SetOwnerCharacter(ACharacter* character)
{
	OwnerCharacter = Cast<ACBaseCharacter>(character);
	MuzzleIndex = OwnerCharacter->GetMesh()->GetBoneIndex("PistolBarrel");
}

void URAbilityActionComponent::OnStartAction()
{
	if (IsCoolTiming)
		return;
	if (!IsAbiliting)
	{
		IsAbiliting = true;
		OwnerCharacter->bUseControllerRotationYaw = true;
		OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
		SetComponentTickEnabled(true);
	}
	else
	{
		IsAbiliting = false;
		OwnerCharacter->bUseControllerRotationYaw = false;
		OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
		SetComponentTickEnabled(false);
	}
}

void URAbilityActionComponent::HologramAction()
{
	IsAbiliting = false;
	SetComponentTickEnabled(false);
	IsCoolTiming = true;
	GetWorld()->GetTimerManager().SetTimer(CoolTimeHandle, this, &URAbilityActionComponent::CoolTimeUpdate, IntervalCoolTime, true);

	OwnerCharacter->PlayAnimMontage(RAbliltyMontage);
}

void URAbilityActionComponent::BeginNotifyAction()
{
	
}

void URAbilityActionComponent::MiddleNotifyAction()
{
	FVector start, end, direction;
	OwnerCharacter->GetLocationAndDirection(start, end, direction, true, 0.3f, 0.4f);

	//UGameplayStatics::SpawnEmitterAttached(FlashParticle, OwnerCharacter->GetMesh(), "PistolBarrel", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true, EPSCPoolMethod::AutoRelease);
	//UGameplayStatics::SpawnEmitterAttached(EjectParticle, OwnerCharacter->GetMesh(), "PistolBarrel", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset);

	FVector muzzleLocation = OwnerCharacter->GetMesh()->GetSocketLocation("PistolBarrel");

	//UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSoundCue, muzzleLocation);


	if (!!BulletClass)
	{
		ACBullet* bullet;
		bullet = Cast<ACBullet>(ObjectPoolFactory->SpawnObject(BulletClass));

		FTransform Transform = bullet->GetTransform();
		Transform.SetLocation(muzzleLocation);
		direction = UKismetMathLibrary::GetDirectionUnitVector(muzzleLocation, end);
		Transform.SetRotation(FQuat(direction.Rotation()));
		bullet->SetActorTransform(Transform);
		bullet->PoolObject->SetActorLifeTime(3.0f);
		if (!(bullet->bInitailized))
		{
			bullet->bInitailized = true;
			bullet->GetMesh()->OnComponentHit.AddDynamic(this, &URAbilityActionComponent::OnHitPaticle);
		}
		bullet->PoolObject->SetActive(true);

	}
}

void URAbilityActionComponent::EndNotifyAction()
{
}
