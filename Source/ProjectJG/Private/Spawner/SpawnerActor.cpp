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

#include "BaseSystem/GameStateBase/InGameStateBase.h"

ASpawnerActor::ASpawnerActor()
{
	CHelpers::CreateActorComponent(this, &BoxComp, "BoxComponent");
	//BoxComp->bHiddenInGame = false;
}

void ASpawnerActor::BeginPlay()
{
	Super::BeginPlay();
	//ObjectPoolFactory = CHelpers::GetComponent<UObjectPoolFactory>(GetWorld()->GetAuthGameMode());

	//if (ObjectPoolFactory != nullptr)
	if(HasAuthority())
	{
		Initailized();
		if(bInitSpawn)
			StartSpawn();
	}
}

bool ASpawnerActor::SpawnCheck()
{
	if (SpawnDataAsset==nullptr)
		return false;

	if (!bEnemysSpawnable)
	{
		return false;
	}
	
	if (bWaitUntilSpawnedEnemysDies)
	{
		if (EnemysSpawnCount > 0)
		{
			return false;
		}
	}
	if (SpawnerSpawnCount > 0)
	{
		--SpawnerSpawnCount;
		GetWorld()->GetTimerManager().SetTimer(SpawnTimeHandle, this, &ASpawnerActor::SpawnActor, SpawnDataAsset->EnemysSpawnDatas[EnemysSpawnLayer].SpawnIntervalTime, false);
		return true;
	}
	else if (SpawnerSpawnCount == -1)
	{
		GetWorld()->GetTimerManager().SetTimer(SpawnTimeHandle, this, &ASpawnerActor::SpawnActor, SpawnDataAsset->EnemysSpawnDatas[EnemysSpawnLayer].SpawnIntervalTime, false);
		return true;
	}
	else if (SpawnerSpawnCount == 0)
	{
		if (bWaitUntilSpawnedLayerEnemysDies)
		{
			if(LayerEnemysMaxCount>0)
				return false;
		}
		SpawnCountInitailized();
		GetWorld()->GetTimerManager().SetTimer(SpawnTimeHandle, this, &ASpawnerActor::SpawnActor, SpawnDataAsset->EnemysSpawnDatas[EnemysSpawnLayer].SpawnIntervalTime, false);
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
			bEnemysSpawnable = false;
			if (!bAllLayerLoopSpawn)
				return;
			bEnemysSpawnable = true;
			EnemysSpawnLayer = 0;
		}
	}
}

void ASpawnerActor::SpawnActor()
{
	UWorld* const World = GetWorld();
	if (SpawnCheck())
	{
		for (int i=0; i < SpawnDataAsset->EnemysSpawnDatas[EnemysSpawnLayer].Enemys.Num(); ++i)
		{
			for (int j=0; j < SpawnDataAsset->EnemysSpawnDatas[EnemysSpawnLayer].Enemys[i].EnemyCount; ++j)
			{
				CheckNull(SpawnDataAsset->EnemysSpawnDatas[EnemysSpawnLayer].Enemys[i].Enemy);
				//AActor* actor = ObjectPoolFactory->SpawnObject(SpawnDataAsset->EnemysSpawnDatas[EnemysSpawnLayer].Enemys[i].Enemy);
				AActor* actor = World->SpawnActor<AActor>(SpawnDataAsset->EnemysSpawnDatas[EnemysSpawnLayer].Enemys[i].Enemy, FVector::ZeroVector, FRotator::ZeroRotator);
				FVector spawnLocation =UKismetMathLibrary::RandomPointInBoundingBox(BoxComp->Bounds.Origin, BoxComp->Bounds.BoxExtent);
				IObjectPoolFunctions* object = Cast<IObjectPoolFunctions>(actor);
				object->OnReturnSpawner.Unbind();
				object->OnReturnSpawner.BindUObject(this, &ASpawnerActor::OnReturnEnemyCount);
				actor->SetActorLocation(spawnLocation);
				//Cast<ABaseEnemyCharacter>(actor)->PoolObject->SetActive(true);
				++EnemysSpawnCount;
			}
		}
		SpawnLayer();
	}
}

void ASpawnerActor::StartSpawn()
{
	bEnemysSpawnable = true;
	GetWorld()->GetTimerManager().SetTimer(SpawnTimeHandle, this, &ASpawnerActor::SpawnActor, SpawnDataAsset->EnemysSpawnDatas[EnemysSpawnLayer].SpawnIntervalTime, false);
}

void ASpawnerActor::StopSpawn()
{
	bEnemysSpawnable = false;
}

void ASpawnerActor::UpdateKillCount()
{
	if(!GameState)
		GameState = Cast<AInGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	CheckNull(GameState);
	GameState->UpdateKillScore();
}

void ASpawnerActor::OnReturnEnemyCount(AActor* actor)
{
	--EnemysSpawnCount;
	--LayerEnemysMaxCount;
	if (EnemysSpawnCount == 0)
	{
		if (bWaitUntilSpawnedLayerEnemysDies)
		{
			if (LayerEnemysMaxCount==0)
			{
				GetWorld()->GetTimerManager().SetTimer(SpawnTimeHandle, this, &ASpawnerActor::SpawnActor, SpawnDataAsset->EnemysSpawnDatas[EnemysSpawnLayer].SpawnIntervalTime, false);

				return;
			}
		}
		if (bWaitUntilSpawnedEnemysDies)
		{
			GetWorld()->GetTimerManager().SetTimer(SpawnTimeHandle, this, &ASpawnerActor::SpawnActor, SpawnDataAsset->EnemysSpawnDatas[EnemysSpawnLayer].SpawnIntervalTime, false);
			return;
		}
	}

	UpdateKillCount();
}

void ASpawnerActor::Initailized()
{
	CheckNull(SpawnDataAsset);
	EnemysSpawnLayer = 0;
	SpawnCountInitailized();
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void ASpawnerActor::SpawnCountInitailized()
{
	SpawnerSpawnCount = SpawnDataAsset->EnemysSpawnDatas[EnemysSpawnLayer].SpawnCount;
	EnemysSpawnCount = 0;
	LayerEnemysMaxCount = 0;
	for (auto Enemy : SpawnDataAsset->EnemysSpawnDatas[EnemysSpawnLayer].Enemys)
	{
		LayerEnemysMaxCount += Enemy.EnemyCount * SpawnerSpawnCount;
	}
}