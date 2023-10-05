
#include "Character/Enemies/RangeEnemy/RangeEnemyCharacter.h"
#include "Global.h"
#include "Character/Enemies/RangeEnemy/RangeEnemyAIController.h"
#include "Components/SphereComponent.h"
#include "Character/CBaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Character/Components/StatusComponent.h"

#include "Animation/AnimMontage.h"
#include "Sound/SoundCue.h"

#include "Materials/MaterialInstanceConstant.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/PrimitiveComponent.h"
#include "Materials/MaterialExpression.h"
#include "Materials/MaterialExpressionWorldPosition.h"

#include "Character/Enemies/RangeEnemy/Components/RangeEnemyWeaponComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "BehaviorTree/BlackboardComponent.h"
//#include "BehaviorTree/BlackboardData.h"
//#include "BehaviorTree/BehaviorTree.h"

void ARangeEnemyCharacter::OnSphereBeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACBaseCharacter* Player = Cast<ACBaseCharacter>(OtherActor);
	//Clog::Log(Player);

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
	CHelpers::GetClass<ARangeEnemyAIController>(&aicontroller, "Blueprint'/Game/Developers/GohyeongJu/Characters/Enemy/RangeEnemy/BP_RangeEnemyAIController.BP_RangeEnemyAIController_C'");
	AIControllerClass = aicontroller;

	CHelpers::CreateActorComponent<URangeEnemyWeaponComponent>(this, &Weapon, "Weapon");
	Weapon->CreateObjectPool();


	UAnimMontage* AnimMontage;
	CHelpers::GetAsset<UAnimMontage>(&AnimMontage, "AnimMontage'/Game/Developers/GohyeongJu/Characters/Enemy/RangeEnemy/Montage/Death_Backward_Montage.Death_Backward_Montage'");
	BaseMontages.Add(backDieMontage, AnimMontage);

	CHelpers::GetAsset<UAnimMontage>(&AnimMontage, "AnimMontage'/Game/Developers/GohyeongJu/Characters/Enemy/RangeEnemy/Montage/Death_Forward_Montage.Death_Forward_Montage'");
	BaseMontages.Add(frontDieMontage, AnimMontage);

}


void ARangeEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	//Clog::Log(Sphere->GetCollisionObjectType());
	//Sphere->SetCollisionObjectType(ECollisionChannel::ECC_OverlapAll_Deprecated);
	//Clog::Log(Sphere->GetCollisionObjectType());
	
	//Clog::Log(BaseMontages[backDieMontage]->GetFName().ToString());
	//Clog::Log(BaseMontages[frontDieMontage]->GetFName().ToString());

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

void ARangeEnemyCharacter::Fire()
{
	Weapon->Fire();
}

void ARangeEnemyCharacter::RegistBlackBoardDatas(UBlackboardComponent* blackboard)
{
	blackboard->SetValueAsObject(BlackboardDataNames::CharacterStatus, Status);
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
				//Clog::Log("Hit Ray");

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
				//Clog::Log("fail Ray Hit");
				if (Elem.Value < 3)
				{
					Elem.Value += 1;
				}
			}
		}
	}
}


void ARangeEnemyCharacter::BeginHitEffect(AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{

	//Clog::Log(Hit.ImpactPoint);
	//Clog::Log("---------------------------------");
	HitImpact = Hit.ImpactPoint;

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

void ARangeEnemyCharacter::SetImpactVectorFrom(FVector& ProjectileVector)
{
	if (UKismetMathLibrary::Dot_VectorVector(GetActorForwardVector(), ProjectileVector) < 0)
	{
		IsHitFromForward = true;
	}
	else
	{
		IsHitFromForward = false;
	}
}

float ARangeEnemyCharacter::TakeDamage(float Damage)
{

	return Super::TakeDamage(Damage);
}

void ARangeEnemyCharacter::DoEffect()
{

	//Clog::Log(EffectValue);
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

void ARangeEnemyCharacter::Die()
{
	//
	//GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//IsFullBody = true;
	//IsDie = true;
	Super::Die();
	StopAnimMontage();
	if (IsHitFromForward)
	{
		Clog::Log("Die Forward");
		PlayAnimMontage(BaseMontages[frontDieMontage]);
	}
	else
	{
		Clog::Log("Die Backrward");
		PlayAnimMontage(BaseMontages[backDieMontage]);
	}
}

void ARangeEnemyCharacter::BeginNotifyAction()
{
}

void ARangeEnemyCharacter::MiddleNotifyAction()
{
}

void ARangeEnemyCharacter::EndNotifyAction()
{
	if (IsDie)
	{
		Clog::Log("IsDie");
		GetMesh()->GetAnimInstance()->Montage_Pause();
		ReturnPool();
	}
}
