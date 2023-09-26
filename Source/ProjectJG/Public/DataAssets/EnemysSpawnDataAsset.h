#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Character/Enemies/BaseEnemyCharacter.h"
#include "EnemysSpawnDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FEnemySpawn
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		TSubclassOf<ABaseEnemyCharacter> Enemy;
	UPROPERTY(EditAnywhere)
		int EnemyCount = 0;
};

USTRUCT(BlueprintType)
struct FEnemysSpawnParam
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		TArray<FEnemySpawn> Enemys;
	UPROPERTY(EditAnywhere)
		int SpawnCount = 0;
	UPROPERTY(EditAnywhere)
		float SpawnIntervalTime = 0.0f;
	UPROPERTY(EditAnywhere)
		float CurSpawnTime = 0.0f;
	UPROPERTY(EditAnywhere)
		float MaxSpawnTime = 0.0f;
	bool IsCoolTiming = false;
	FTimerHandle SpawnTimeHandle;
};


UCLASS()
class PROJECTJG_API UEnemysSpawnDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<FEnemysSpawnParam> EnemysSpawnDatas;
};
