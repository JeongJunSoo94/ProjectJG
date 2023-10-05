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
	void SpawnActor();
public:	
	virtual void Tick(float DeltaTime) override;
public:
	UPROPERTY(BlueprintReadOnly,EditAnywhere, Category = "Spawns")
		UEnemysSpawnDataAsset* SpawnDataAsset;
	UPROPERTY(EditAnywhere)
		bool IsInfiniteSpawn = false;
		bool IsEnemysSpawnable = true;
		int EnemysSpawnCount=0;
		FTimerHandle SpawnTimeHandle;
};
