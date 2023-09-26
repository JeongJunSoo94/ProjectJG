#include "Spawner/SpawnerActor.h"
#include "Global.h"
#include "Character/Enemies/BaseEnemyCharacter.h"
#include "BaseSystem/InGameModeBase.h"
#include "BaseSystem/ObjectPoolFactory.h"
#include "BaseSystem/PoolObjectActorComponent.h"
#include "DataAssets/EnemysSpawnDataAsset.h"
#include "Character/Enemies/BaseEnemyCharacter.h"

ASpawnerActor::ASpawnerActor()
{
}

void ASpawnerActor::BeginPlay()
{
	Super::BeginPlay();
	ObjectPoolFactory = CHelpers::GetComponent<UObjectPoolFactory>(GetWorld()->GetAuthGameMode());
	if (ObjectPoolFactory != nullptr)
	{
		GetWorld()->GetTimerManager().SetTimer(SpawnDataAsset->EnemysSpawnDatas[0].SpawnTimeHandle, this, &ASpawnerActor::SpawnActor, SpawnDataAsset->EnemysSpawnDatas[0].SpawnIntervalTime, IsInfiniteSpawn);
	}

}

void ASpawnerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnerActor::SpawnActor()
{
	if (SpawnDataAsset)
	{
		for (int i=0; i < SpawnDataAsset->EnemysSpawnDatas[0].Enemys.Num(); ++i)
		{
			for (int j=0; j < SpawnDataAsset->EnemysSpawnDatas[0].Enemys[i].EnemyCount; ++j)
			{
				CheckNull(SpawnDataAsset->EnemysSpawnDatas[0].Enemys[i].Enemy);
				AActor* actor = ObjectPoolFactory->SpawnObject(SpawnDataAsset->EnemysSpawnDatas[0].Enemys[i].Enemy);
				actor->SetActorLocation(GetActorLocation());
				Cast<ABaseEnemyCharacter>(actor)->PoolObject->SetActive(true);
			}
		}
	}
}