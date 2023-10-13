#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataAssets/EnemysSpawnDataAsset.h"
#include "SpawnerActor.generated.h"

UCLASS()
class PROJECTJG_API ASpawnerActor : public AActor
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleDefaultsOnly)
		class UBoxComponent* BoxComp;
public:	
	ASpawnerActor();

protected:
	virtual void BeginPlay() override;
	UPROPERTY(VisibleDefaultsOnly)
		class UObjectPoolFactory* ObjectPoolFactory;
	void OnReturnEnemyCount(AActor* actor);
	bool SpawnCheck();
	void SpawnLayer();
	void SpawnActor();
	void SpawnCountInitailized();
public:	
	void Initailized();
	void StartSpawn();
	void StopSpawn();
public:
	UPROPERTY(BlueprintReadOnly,EditAnywhere, Category = "Spawns")
		UEnemysSpawnDataAsset* SpawnDataAsset;

	UPROPERTY(EditAnywhere, Category = "Spawns")
		bool bAllLayerLoopSpawn = false;

	UPROPERTY(EditAnywhere, Category = "Spawns")
		bool bWaitUntilSpawnedLayerEnemysDies = false;

	UPROPERTY(EditAnywhere, Category = "Spawns")
		bool bWaitUntilSpawnedEnemysDies = false;

	UPROPERTY(EditAnywhere, Category = "Spawns")
		bool bInitSpawn = false;
protected:
		bool bEnemysSpawnable = true;
		int EnemysSpawnCount=0;
		int SpawnerSpawnCount = 0;
		int EnemysSpawnLayer = 0;
		int LayerEnemysMaxCount = 0;
		FTimerHandle SpawnTimeHandle;

		class AInGameStateBase* GameState;
public:
	void UpdateKillCount();
};
