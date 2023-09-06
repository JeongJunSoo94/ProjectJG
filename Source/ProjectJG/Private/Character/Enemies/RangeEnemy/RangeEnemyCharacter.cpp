
#include "Character/Enemies/RangeEnemy/RangeEnemyCharacter.h"
#include "Global.h"
#include "Character/Enemies/RangeEnemy/RangeEnemyAIController.h"
#include "Components/SphereComponent.h"
#include "Character/CBaseCharacter.h"
#include "Components/CapsuleComponent.h"

#include "Animation/AnimMontage.h"
#include "particles/PxParticleSystem.h"
#include "Particles/ParticleSystem.h"
#include "Engine/StaticMeshActor.h"
#include "Sound/SoundCue.h"
#include "Components/StaticMeshComponent.h"
#include "Character/Enemies/RangeEnemy/Bullets/RangeEnemy_Bullet.h"
#include "BaseSystem/ObjectPoolFactory.h"

#include "Materials/MaterialInstanceConstant.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/PrimitiveComponent.h"
#include "Materials/MaterialExpression.h"
#include "Materials/MaterialExpressionWorldPosition.h"

void ARangeEnemyCharacter::OnSphereBeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACBaseCharacter* Player = Cast<ACBaseCharacter>(OtherActor);
	Clog::Log(Player);

	if (Player == nullptr)
	{
		return;
	}


	if (!PlayerAgrroMap.Find(OtherActor))
	{
		PlayerAgrroMap.Add(OtherActor, 0);
	}
	else
	{
		PlayerAgrroMap[OtherActor] = 0;
	}

}

void ARangeEnemyCharacter::OnSphereEndOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (PlayerAgrroMap.Find(OtherActor))
	{
		PlayerAgrroMap[OtherActor] = -1;
	}
}

void ARangeEnemyCharacter::OnHitPaticle(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	FRotator rotator = Hit.ImpactNormal.Rotation();
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, Hit.Location, rotator, true, EPSCPoolMethod::AutoRelease);
	
	IDamageable* character = Cast<IDamageable>(OtherActor);
	CheckNull(character);
	character->TakeDamage(10.0f);
	character->BeginHitEffect(NormalImpulse, Hit);
}

ARangeEnemyCharacter::ARangeEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/ParagonWraith/Characters/Heroes/Wraith/Meshes/Wraith.Wraith'");
	GetMesh()->SetSkeletalMesh(mesh);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	CHelpers::GetAsset<UMaterial>(&Hit_material, "Material'/Game/Developers/GohyeongJu/Characters/Enemy/RangeEnemy/Materials/Hit_Material.Hit_Material'");


	CHelpers::CreateComponent<USphereComponent>(this, &Sphere, "Sphere", GetCapsuleComponent());
	Sphere->bHiddenInGame = false;
	Sphere->SetSphereRadius(2000.0f);
	

	TSubclassOf<ARangeEnemyAIController> aicontroller;
	CHelpers::GetClass<ARangeEnemyAIController>(&aicontroller, "Blueprint'/Game/Developers/GohyeongJu/Characters/Enemy/RangeEnemy/RangeEnemyAIController.RangeEnemyAIController_C'");
	AIControllerClass = aicontroller;

	CHelpers::GetAsset<UAnimMontage>(&FireMontage, "AnimMontage'/Game/Developers/GohyeongJu/Characters/Enemy/RangeEnemy/Montage/RangeEnemy_Fire_Montage.RangeEnemy_Fire_Montage'");

	CHelpers::GetAsset<UParticleSystem>(&FlashParticle, "ParticleSystem'/Game/ParagonWraith/FX/Particles/Abilities/Primary/FX/P_Wraith_Primary_MuzzleFlash.P_Wraith_Primary_MuzzleFlash'");
	//CHelpers::GetAsset<UParticleSystem>(&EjectParticle, "");

	CHelpers::GetAsset<USoundCue>(&FireSoundCue, "SoundCue'/Game/Developers/GohyeongJu/Characters/Enemy/RangeEnemy/Sound/RangeEnemy_fire_Cue.RangeEnemy_fire_Cue'");

	CHelpers::GetClass<ARangeEnemy_Bullet>(&BulletClass, "Blueprint'/Game/Developers/GohyeongJu/Characters/Enemy/RangeEnemy/Bullet/RangeEnemy_Bullet.RangeEnemy_Bullet_C'");

	CHelpers::GetAsset<UParticleSystem>(&ImpactParticle, "ParticleSystem'/Game/ParagonWraith/FX/Particles/Abilities/Primary/FX/P_Wraith_Primary_HitCharacter.P_Wraith_Primary_HitCharacter'");

	CHelpers::CreateActorComponent<UObjectPoolFactory>(this, &ObjectPoolFactory, "ObjectPoolFactory");

	ObjectPoolFactory->PoolSize = 20;

	ObjectPoolFactory->PooledObjectSubclass = BulletClass;
	ObjectPoolFactory->Initialized();
}


void ARangeEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	//Clog::Log(Sphere->GetCollisionObjectType());
	//Sphere->SetCollisionObjectType(ECollisionChannel::ECC_OverlapAll_Deprecated);
	//Clog::Log(Sphere->GetCollisionObjectType());
	
	GetWorldTimerManager().SetTimer(RayTimer,this, &ARangeEnemyCharacter::RayToPlayer, 1.0f, true);
	
	CollisionParams.AddIgnoredActor(this->GetOwner());

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ARangeEnemyCharacter::OnSphereBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ARangeEnemyCharacter::OnSphereEndOverlap);
}

void ARangeEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsDoEffect)
	{
		DoEffect();
	}

}

AActor* ARangeEnemyCharacter::GetPriorityTarget()
{
	if (PlayerAgrroMap.Num() <= 0)
	{
		//Clog::Log("PlayerAgrroMap is Zero");

		return nullptr;
	}

	int MaxValue = 0;
	AActor* TargetActor = nullptr;

	for (auto& Elem : PlayerAgrroMap)
	{

		if (Elem.Value > MaxValue)
		{
			if (Elem.Value == 3)
			{
				return Elem.Key;
			}
			TargetActor = Elem.Key;
			MaxValue = Elem.Value;
		}
	}
	
	return TargetActor;
}

void ARangeEnemyCharacter::RayToPlayer()
{
	if (PlayerAgrroMap.Num() <= 0)
		return;

	for (auto& Elem : PlayerAgrroMap)
	{
		if (Elem.Value >= 0)
		{
			FHitResult OutHit;
			FVector Start = GetActorLocation();
			FVector End = Elem.Key->GetActorLocation();
			//FVector Extension = End - Start;
			//Extension.Normalize(1.0f);
			DrawDebugLine(GetWorld(), Start, End , FColor::Red, false, 1, 0, 1);
			
			bool IsHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);

			if (IsHit)
			{
				Clog::Log("Hit Ray");

				double remainSquaredDistance = ((End - Start).SquaredLength() - OutHit.Distance * OutHit.Distance);
				
				if (remainSquaredDistance < MaxRemainSquaredDistance && Elem.Value < 3)
				{
					Elem.Value += 1;
				}
				else
				{
					if (Elem.Value > 0)
					{
						Elem.Value -= 1;
					}
				}
			}
			else
			{
				Clog::Log("fail Ray Hit");
				if (Elem.Value < 3)
				{
					Elem.Value += 1;
				}
			}
		}
	}
}


void ARangeEnemyCharacter::Fire()
{

	Clog::Log("Play Fire");



	PlayAnimMontage(FireMontage);

	FVector start, end, direction;

	direction = GetActorForwardVector();
	UGameplayStatics::SpawnEmitterAttached(FlashParticle, GetMesh(), "Muzzle_01", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true, EPSCPoolMethod::AutoRelease);
	//UGameplayStatics::SpawnEmitterAttached(EjectParticle, GetMesh(), "Muzzle_01", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true, EPSCPoolMethod::AutoRelease);


	FVector muzzleLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	//muzzleLocation += 400 * direction;
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSoundCue, muzzleLocation);


	if (!!BulletClass)
	{
		ARangeEnemy_Bullet* bullet;
		bullet = Cast<ARangeEnemy_Bullet>(ObjectPoolFactory->SpawnObject());

		//bullet->TeleportTo(muzzleLocation, direction.Rotation());
		FTransform Transform = bullet->GetTransform();
		Transform.SetLocation(muzzleLocation);
		Transform.SetRotation(FQuat(direction.Rotation()));
		bullet->SetActorTransform(Transform);
		bullet->SetActorLifeTime(3.0f);

		if (!(bullet->bInitailized))
		{
			//Clog::Log("Bind");
			bullet->bInitailized = true;
			bullet->GetMesh()->OnComponentHit.AddDynamic(this, &ARangeEnemyCharacter::OnHitPaticle);
		}
		bullet->SetActive(true);

		//ACBullet* bullet= GetWorld()->SpawnActor<ACBullet>(BulletClass, muzzleLocation, direction.Rotation());
		//bullet->GetMesh()->OnComponentHit.AddDynamic(this, &UCLtBelicaWeapon::OnHitPaticle);
	}
	//direction = OwnerCharacter->GetMesh()->GetSocketTransform("SMG_Barrel").GetRotation();

}

void ARangeEnemyCharacter::BeginHitEffect( FVector NormalImpulse, const FHitResult& Hit)
{
	Clog::Log(Hit.ImpactPoint);
	Clog::Log("---------------------------------");

	if (IsDoEffect)
		return;

	//UMaterialInterface* m_interface = GetMesh()->GetMaterial(4);

	if (HitMaterial_Dynamics.Num() < 4)//나중에 수정 해야함
	{
		HitMaterial_Dynamics.Add(GetMesh()->CreateDynamicMaterialInstance(0));
		HitMaterial_Dynamics.Add(GetMesh()->CreateDynamicMaterialInstance(1));
		HitMaterial_Dynamics.Add(GetMesh()->CreateDynamicMaterialInstance(2));
		HitMaterial_Dynamics.Add(GetMesh()->CreateDynamicMaterialInstance(3));
	}
	HitImpact = Hit.ImpactPoint;
	// 움직일때 같이 움직이도록 해야함.
	//Clog::Log(HitMaterial_Dynamic);
	for (UMaterialInstanceDynamic*& HitMaterial_Dynamic : HitMaterial_Dynamics)
	{
		HitMaterial_Dynamic->SetVectorParameterValue(TEXT("Pos"), HitImpact);
		HitMaterial_Dynamic->SetScalarParameterValue(TEXT("Emissive"), 50.0f);
		HitMaterial_Dynamic->SetScalarParameterValue(TEXT("Fade"), 30.0f);
	}
	//HitMaterial_Dynamic->

	//GetWorldTimerManager().SetTimer(EffectTimer, 10.0f, false);
	IsDoEffect = true;
	EffectValue = 0.0f;
}

float ARangeEnemyCharacter::TakeDamage(float Damage)
{

	return Super::TakeDamage(Damage);
}

void ARangeEnemyCharacter::DoEffect()
{

	Clog::Log(EffectValue);
	for (UMaterialInstanceDynamic*& HitMaterial_Dynamic : HitMaterial_Dynamics)
	{
		HitMaterial_Dynamic->SetScalarParameterValue(TEXT("Radius"), EffectValue);
	}
	
	EffectValue += 0.3f;

	if (EffectValue >= 300.0f)
		IsDoEffect = false;
	//TArray<FMaterialParameterInfo> paramInfoArray;
	//TArray<FGuid> g_uidArray;

	//HitMaterial_Dynamic->GetAllScalarParameterInfo(paramInfoArray, g_uidArray);



}