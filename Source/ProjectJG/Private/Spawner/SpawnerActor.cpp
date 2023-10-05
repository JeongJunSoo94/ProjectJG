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
		GetWorld()->GetTimerManager().SetTimer(SpawnTimeHandle, this, &ASpawnerActor::SpawnActor, SpawnDataAsset->EnemysSpawnDatas[EnemysSpawnLayer].SpawnIntervalTime, IsInfiniteSpawn);
		SpawnerSpawnCount = SpawnDataAsset->EnemysSpawnDatas[EnemysSpawnLayer].SpawnCount;
	}

}

void ASpawnerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ASpawnerActor::SpawnCheck()
{
	if (SpawnDataAsset==nullptr)
		return false;
	if (IsInfiniteSpawn)
	{
		--SpawnerSpawnCount;
		return true;
	}

	if (SpawnerSpawnCount > 0)
	{
		IsEnemysSpawnable = false;
		--SpawnerSpawnCount;
		return true;
	}
	else if (SpawnerSpawnCount == -1)
	{
		return true;
	}
	return false;
}

void ASpawnerActor::SpawnLayer()
{
	if (SpawnerSpawnCount == 0)
	{
		if (SpawnDataAsset->EnemysSpawnDatas.Num() > EnemysSpawnLayer + 1)
			++EnemysSpawnLayer;
		else
		{
			if (!IsInfiniteSpawn)
				return;
			EnemysSpawnLayer = 0;
		}
		SpawnerSpawnCount = SpawnDataAsset->EnemysSpawnDatas[EnemysSpawnLayer].SpawnCount;
		if (IsInfiniteSpawn)
		{
			GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
			GetWorld()->GetTimerManager().SetTimer(SpawnTimeHandle, this, &ASpawnerActor::SpawnActor, SpawnDataAsset->EnemysSpawnDatas[EnemysSpawnLayer].SpawnIntervalTime, IsInfiniteSpawn);
		}
	}
}

void ASpawnerActor::SpawnActor()
{
	if (!IsEnemysSpawnable)
	{
		return;
	}

	if (SpawnCheck())
	{
		for (int i=0; i < SpawnDataAsset->EnemysSpawnDatas[EnemysSpawnLayer].Enemys.Num(); ++i)
		{
			for (int j=0; j < SpawnDataAsset->EnemysSpawnDatas[EnemysSpawnLayer].Enemys[i].EnemyCount; ++j)
			{
				CheckNull(SpawnDataAsset->EnemysSpawnDatas[EnemysSpawnLayer].Enemys[i].Enemy);
				AActor* actor = ObjectPoolFactory->SpawnObject(SpawnDataAsset->EnemysSpawnDatas[EnemysSpawnLayer].Enemys[i].Enemy);
				FVector spawnLocation =UKismetMathLibrary::RandomPointInBoundingBox(BoxComp->Bounds.Origin, BoxComp->Bounds.BoxExtent);
				IObjectPoolFunctions* object = Cast<IObjectPoolFunctions>(actor);
				object->OnReturnSpawner.Unbind();
				object->OnReturnSpawner.BindUObject(this, &ASpawnerActor::OnReturnEnemyCount);
				actor->SetActorLocation(spawnLocation);
				Cast<ABaseEnemyCharacter>(actor)->PoolObject->SetActive(true);
				++EnemysSpawnCount;
			}
		}
		SpawnLayer();
	}
}


void ASpawnerActor::OnReturnEnemyCount(AActor* actor)
{
	--EnemysSpawnCount;
	if (EnemysSpawnCount == 0)
	{
		if (!IsInfiniteSpawn)
		{
			IsEnemysSpawnable = true;
			GetWorld()->GetTimerManager().SetTimer(SpawnTimeHandle, this, &ASpawnerActor::SpawnActor, SpawnDataAsset->EnemysSpawnDatas[EnemysSpawnLayer].SpawnIntervalTime, false);
		}
	}
}