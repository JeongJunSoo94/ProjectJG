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
public:	
	void Initailized();
	void StartSpawn();
	void StopSpawn();
public:
	UPROPERTY(BlueprintReadOnly,EditAnywhere, Category = "Spawns")
		UEnemysSpawnDataAsset* SpawnDataAsset;
	UPROPERTY(EditAnywhere, Category = "Spawns")
		bool IsInfiniteSpawn = false;
	UPROPERTY(EditAnywhere, Category = "Spawns")
		bool IsAutoSpawn = false;
	UPROPERTY(EditAnywhere, Category = "Spawns")
		bool IsInitSpawn = false;
protected:
		bool IsEnemysSpawnable = true;
		int EnemysSpawnCount=0;
		int SpawnerSpawnCount = 0;
		int EnemysSpawnLayer = 0;
		FTimerHandle SpawnTimeHandle;
};
