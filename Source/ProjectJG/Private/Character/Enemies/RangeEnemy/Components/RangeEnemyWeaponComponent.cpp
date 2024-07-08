

#include "Character/Enemies/RangeEnemy/Components/RangeEnemyWeaponComponent.h"
#include "Global.h"

#include "Character/Enemies/BaseEnemyCharacter.h"

#include "Animation/AnimMontage.h"
#include "Sound/SoundCue.h"
#include "particles/PxParticleSystem.h"
#include "Particles/ParticleSystem.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "Character/Enemies/RangeEnemy/Bullets/RangeEnemy_Bullet.h"
#include "BaseSystem/ObjectPoolFactory.h"
#include "BaseSystem/InGameModeBase.h"
#include "BaseSystem/PoolObjectActorComponent.h"

#include "Bullet/Projectile.h"

void URangeEnemyWeaponComponent::OnHitPaticle(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	FRotator rotator = Hit.ImpactNormal.Rotation();
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, Hit.Location, rotator, true, EPSCPoolMethod::AutoRelease);

	//IDamageable* character = Cast<IDamageable>(OtherActor);
	//CheckNull(character);
	//character->TakeDamage(10.0f);
	//character->BeginHitEffect(OwnerCharacter,NormalImpulse, Hit);
}

URangeEnemyWeaponComponent::URangeEnemyWeaponComponent()
{

	CHelpers::GetAsset<UAnimMontage>(&FireMontage, "AnimMontage'/Game/Developers/GohyeongJu/Characters/Enemy/RangeEnemy/Montage/RangeEnemy_Fire_Montage.RangeEnemy_Fire_Montage'");

	CHelpers::GetAsset<UParticleSystem>(&FlashParticle, "ParticleSystem'/Game/ParagonWraith/FX/Particles/Abilities/Primary/FX/P_Wraith_Primary_MuzzleFlash.P_Wraith_Primary_MuzzleFlash'");
	//CHelpers::GetAsset<UParticleSystem>(&EjectParticle, "");

	CHelpers::GetAsset<USoundCue>(&FireSoundCue, "SoundCue'/Game/Developers/GohyeongJu/Characters/Enemy/RangeEnemy/Sound/RangeEnemy_fire_Cue.RangeEnemy_fire_Cue'");

	//CHelpers::GetClass<ARangeEnemy_Bullet>(&BulletClass, "Blueprint'/Game/Developers/JJS/Weapons/Projectiles/BP_ProjectileBullet.BP_ProjectileBullet_C'");

	CHelpers::GetAsset<UParticleSystem>(&ImpactParticle, "ParticleSystem'/Game/ParagonWraith/FX/Particles/Abilities/Primary/FX/P_Wraith_Primary_HitCharacter.P_Wraith_Primary_HitCharacter'");
}

void URangeEnemyWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	if (OwnerCharacter == nullptr)
		OwnerCharacter = Cast<ABaseEnemyCharacter>(GetOwner());

	//ObjectPoolFactory = CHelpers::GetComponent<UObjectPoolFactory>(GetWorld()->GetAuthGameMode());
	//if (ObjectPoolFactory != nullptr)
	//{
	//	ObjectPoolFactory->CreateObject(20, BulletClass);
	//}
}

void URangeEnemyWeaponComponent::Fire()
{
	Clog::Log("Play Fire");

	OwnerCharacter->PlayAnimMontage(FireMontage);

	FVector start, end, direction;

	direction = OwnerCharacter->GetActorForwardVector();
	UGameplayStatics::SpawnEmitterAttached(FlashParticle, OwnerCharacter->GetMesh(), "Muzzle_01", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true, EPSCPoolMethod::AutoRelease);
	//UGameplayStatics::SpawnEmitterAttached(EjectParticle, GetMesh(), "Muzzle_01", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true, EPSCPoolMethod::AutoRelease);


	FVector muzzleLocation = OwnerCharacter->GetMesh()->GetSocketLocation("Muzzle_01");
	//muzzleLocation += 400 * direction;
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSoundCue, muzzleLocation);

	UWorld* World = GetWorld();
	if (!!ProjectileClass)
	{
		APawn* InstigatorPawn = Cast<APawn>(GetOwner());

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = InstigatorPawn;

		AProjectile* bullet;
		bullet = World->SpawnActor<AProjectile>(ProjectileClass, muzzleLocation, direction.Rotation(), SpawnParams);//Cast<ARangeEnemy_Bullet>(ObjectPoolFactory->SpawnObject(BulletClass));
		bullet->Damage = 10.0f;
		//bullet->TeleportTo(muzzleLocation, direction.Rotation());
		//FTransform Transform = bullet->GetTransform();
		//Transform.SetLocation(muzzleLocation);
		//Transform.SetRotation(FQuat(direction.Rotation()));
		//bullet->SetActorTransform(Transform);
		//bullet->PoolObject->SetActorLifeTime(3.0f);

		//if (!(bullet->bInitailized))
		//{
		//	//Clog::Log("Bind");
		//	bullet->bInitailized = true;
		//	bullet->GetMesh()->OnComponentHit.AddDynamic(this, &URangeEnemyWeaponComponent::OnHitPaticle);
		//}
		//bullet->PoolObject->SetActive(true);

		//ACBullet* bullet= GetWorld()->SpawnActor<ACBullet>(BulletClass, muzzleLocation, direction.Rotation());
		//bullet->GetMesh()->OnComponentHit.AddDynamic(this, &UCLtBelicaWeapon::OnHitPaticle);
	}
	//direction = OwnerCharacter->GetMesh()->GetSocketTransform("SMG_Barrel").GetRotation();

}

//void URangeEnemyWeaponComponent::CreateObjectPool()
//{
//	//if(OwnerCharacter==nullptr)
//	//	OwnerCharacter = Cast<ABaseEnemyCharacter>(GetOwner());
//
//	//CHelpers::CreateActorComponent<UObjectPoolFactory>(OwnerCharacter, &ObjectPoolFactory, "ObjectPoolFactory");
//
//	//ObjectPoolFactory->PoolSize = 20;
//
//	//ObjectPoolFactory->PooledObjectSubclass = BulletClass;
//	//ObjectPoolFactory->Initialized();
//
//}
