#include "WorldObjects/Item/ItemSpawnPoint.h"
#include "WorldObjects/Item/Item.h"

// Sets default values
AItemSpawnPoint::AItemSpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

// Called when the game starts or when spawned
void AItemSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	StartSpawnItemTimer((AActor*)nullptr);
}

// Called every frame
void AItemSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemSpawnPoint::SpawnItem()
{
	int32 NumItemClasses = ItemClasses.Num();
	if (NumItemClasses > 0)
	{
		int32 Selection = FMath::RandRange(0, NumItemClasses - 1);
		SpawnedItem = GetWorld()->SpawnActor<AItem>(ItemClasses[Selection], GetActorTransform());

		if (HasAuthority() && SpawnedItem)
		{
			SpawnedItem->OnDestroyed.AddDynamic(this, &AItemSpawnPoint::StartSpawnItemTimer);
		}
	}
}

void AItemSpawnPoint::SpawnItemTimerFinished()
{
	if (HasAuthority())
	{
		SpawnItem();
	}
}

void AItemSpawnPoint::StartSpawnItemTimer(AActor* DestroyedActor)
{
	const float SpawnTime = FMath::FRandRange(SpawnItemTimeMin, SpawnItemTimeMax);
	GetWorldTimerManager().SetTimer(
		SpawnItemTimer,
		this,
		&AItemSpawnPoint::SpawnItemTimerFinished,
		SpawnTime
	);
}
