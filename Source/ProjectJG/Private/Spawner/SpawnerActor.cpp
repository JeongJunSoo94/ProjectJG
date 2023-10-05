#include "Spawner/SpawnerActor.h"
#include "Global.h"
#include "Character/Enemies/BaseEnemyCharacter.h"
#include "BaseSystem/InGameModeBase.h"
#include "BaseSystem/ObjectPoolFactory.h"
#include "BaseSystem/PoolObjectActorComponent.h"
#include "DataAssets/EnemysSpawnDataAsset.h"
#include "Character/Enemies/BaseEnemyCharacter.h"
#include "Components/BoxComponent.h"
#include "BaseSystem/ObjectPoolFunctions.h"

ASpawnerActor::ASpawnerActor()
{
	CHelpers::CreateActorComponent(this, &BoxComp, "BoxComponent");
	BoxComp->bHiddenInGame = false;
}

void ASpawnerActor::BeginPlay()
{
	Super::BeginPlay();
	ObjectPoolFactory = CHelpers::GetComponent<UObjectPoolFactory>(GetWorld()->GetAuthGameMode());
	if (ObjectPoolFactory != nullptr)
	{
		GetWorld()->GetTimerManager().SetTimer(SpawnTimeHandle, this, &ASpawnerActor::SpawnActor, SpawnDataAsset->EnemysSpawnDatas[0].SpawnIntervalTime, IsInfiniteSpawn);
	}

}

void ASpawnerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnerActor::SpawnActor()
{
	Clog::Log("A");
	if (!IsEnemysSpawnable)
	{
		return;
	}
	if (SpawnDataAsset)
	{
		Clog::Log("B");
		for (int i=0; i < SpawnDataAsset->EnemysSpawnDatas[0].Enemys.Num(); ++i)
		{
			for (int j=0; j < SpawnDataAsset->EnemysSpawnDatas[0].Enemys[i].EnemyCount; ++j)
			{
				CheckNull(SpawnDataAsset->EnemysSpawnDatas[0].Enemys[i].Enemy);
				AActor* actor = ObjectPoolFactory->SpawnObject(SpawnDataAsset->EnemysSpawnDatas[0].Enemys[i].Enemy);
				FVector spawnLocation =UKismetMathLibrary::RandomPointInBoundingBox(BoxComp->Bounds.Origin, BoxComp->Bounds.BoxExtent);
				IObjectPoolFunctions* object = Cast<IObjectPoolFunctions>(actor);
				object->OnReturnSpawner.Unbind();
				object->OnReturnSpawner.BindUObject(this, &ASpawnerActor::OnReturnEnemyCount);
				actor->SetActorLocation(spawnLocation);
				Cast<ABaseEnemyCharacter>(actor)->PoolObject->SetActive(true);
				++EnemysSpawnCount;
			}
		}
		IsEnemysSpawnable = false;
	}
}

void ASpawnerActor::OnReturnEnemyCount(AActor* actor)
{
	--EnemysSpawnCount;
	if (EnemysSpawnCount == 0)
	{
		IsEnemysSpawnable = true;
	}
}