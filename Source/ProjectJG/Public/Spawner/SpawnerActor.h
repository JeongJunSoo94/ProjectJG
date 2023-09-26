#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataAssets/EnemysSpawnDataAsset.h"
#include "SpawnerActor.generated.h"

UCLASS()
class PROJECTJG_API ASpawnerActor : public AActor
{
	GENERATED_BODY()
public:	
	ASpawnerActor();

protected:
	virtual void BeginPlay() override;
	UPROPERTY(VisibleDefaultsOnly)
		class UObjectPoolFactory* ObjectPoolFactory;
	void SpawnActor();
public:	
	virtual void Tick(float DeltaTime) override;
public:
	UPROPERTY(BlueprintReadOnly,EditAnywhere, Category = "Spawns")
		UEnemysSpawnDataAsset* SpawnDataAsset;
	UPROPERTY(EditAnywhere)
		bool IsInfiniteSpawn = false;
};
