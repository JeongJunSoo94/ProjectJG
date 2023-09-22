#include "WorldObjects/Traps/TestTrapActor.h"
#include "Global.h"
#include "BaseSystem/ObjectPoolFactory.h"
#include "Bullet/CBullet.h"
#include "Character/CBaseCharacter.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

ATestTrapActor::ATestTrapActor()
{
	CHelpers::GetClass<ACBullet>(&BulletClass, "Blueprint'/Game/Developers/USER/Bullet/BP_CTrapBullet.BP_CTrapBullet_C'");
	CHelpers::GetAsset<UBehaviorTree>(&BT, "BehaviorTree'/Game/Developers/USER/WorldObjects/TrapBehaviorTree.TrapBehaviorTree'");
	CHelpers::GetAsset<UBlackboardData>(&BTData, "BlackboardData'/Game/Developers/USER/WorldObjects/TrapBlackboardData.TrapBlackboardData'");

	//CHelpers::CreateActorComponent<UObjectPoolFactory>(this, &ObjectPoolFactory, "ObjectPoolFactory");
	CHelpers::CreateActorComponent<UBehaviorTreeComponent>(this, &BTC, "BTC");
	CHelpers::CreateActorComponent<UBlackboardComponent>(this, &BlC, "BlC");

}

void ATestTrapActor::BeginPlay()
{
	Super::BeginPlay();
	//ObjectPoolFactory->PoolSize = 20;
	//ObjectPoolFactory->PooledObjectSubclass = BulletClass;
	//ObjectPoolFactory->Initialized();
	
	GetWorldTimerManager().SetTimer(LifeTimer, this, &ATestTrapActor::Fire, 1.0f, true);
	BT->BlackboardAsset = BTData;
	
	BTC->StartTree(*BT);
}

void ATestTrapActor::OnHitPlayer(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ACBaseCharacter* character = Cast<ACBaseCharacter>(OtherActor);
	CheckNull(character);
	character->TakeDamage(10.0f);
	//FRotator rotator = Hit.ImpactNormal.Rotation();
	//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, Hit.Location, rotator, true, EPSCPoolMethod::AutoRelease);
	//UGameplayStatics::SpawnDecalAtLocation(GetWorld(), DecalMaterial, FVector(5), Hit.Location, rotator, 10.0f);
}

void ATestTrapActor::Fire()
{
	FVector muzzleLocation = GetActorLocation();
	FRotator direction = GetActorRotation();
	if (!!BulletClass)
	{
		//ACBullet* bullet;
		//bullet = Cast<ACBullet>(ObjectPoolFactory->SpawnObject());
		//FTransform Transform = bullet->GetTransform();
		//Transform.SetLocation(muzzleLocation);
		//Transform.SetRotation(FQuat(direction));
		//bullet->SetActorTransform(Transform);
		//bullet->SetActorLifeTime(3.0f);
		//if (!(bullet->bInitailized))
		//{
		//	bullet->bInitailized = true;
		//	bullet->GetMesh()->OnComponentHit.AddDynamic(this, &ATestTrapActor::OnHitPlayer);
		//}
		//bullet->SetActive(true);
	}
}

