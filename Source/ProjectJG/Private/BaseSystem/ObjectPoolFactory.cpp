#include "BaseSystem/ObjectPoolFactory.h"
#include "Global.h"
#include "BaseSystem/BasePooledObject.h"
#include "BaseSystem/PoolObjectActorComponent.h"

UObjectPoolFactory::UObjectPoolFactory()
{
}

void UObjectPoolFactory::BeginPlay()
{
	Super::BeginPlay();
}

void UObjectPoolFactory::CreateObject(unsigned int createCount, TSubclassOf<class AActor> pooledObjectSubclass)
{
	CheckNull(pooledObjectSubclass);
	UWorld* const World = GetWorld();
	CheckNull(World);
	for (unsigned int i = 0; i < createCount; ++i)
	{
		AActor* actor = World->SpawnActor<AActor>(pooledObjectSubclass, FVector::ZeroVector, FRotator::ZeroRotator);
		UPoolObjectActorComponent* PooledObject = CHelpers::GetComponent<UPoolObjectActorComponent>(actor);
		if (PooledObject != nullptr)
		{
			PooledObject->SetActive(false);
			if (ObjectPools.Find(pooledObjectSubclass->GetFName())==nullptr)
			{
				ObjectPools.Add(pooledObjectSubclass->GetFName(), FActorArray());
				ObjectPools[pooledObjectSubclass->GetFName()].SpawnPoolIndex = ObjectPools.Num() - 1;
				AvailableObjectPools.SetNum(AvailableObjectPools.Num()+1);
			}
			PooledObject->SetPoolIndex(ObjectPools[pooledObjectSubclass->GetFName()].actorArray.Num(), pooledObjectSubclass->GetFName());
			PooledObject->OnReturnToPool.BindUObject(this, &UObjectPoolFactory::OnReturnToPool);
			ObjectPools[pooledObjectSubclass->GetFName()].actorArray.Add(actor);
			
			AvailableObjectPools[ObjectPools[pooledObjectSubclass->GetFName()].SpawnPoolIndex].Enqueue(actor);
		}
	}
}

AActor* UObjectPoolFactory::SpawnObject(TSubclassOf<class AActor> pooledObjectSubclass)
{
	AActor* PooledObject=nullptr;
	if (ObjectPools.Find(pooledObjectSubclass->GetFName())==nullptr)
	{
		CreateObject(1, pooledObjectSubclass);
	}
	else if (AvailableObjectPools[ObjectPools[pooledObjectSubclass->GetFName()].SpawnPoolIndex].IsEmpty())
	{
		CreateObject(1, pooledObjectSubclass);
	}
	AvailableObjectPools[ObjectPools[pooledObjectSubclass->GetFName()].SpawnPoolIndex].Dequeue(PooledObject);
	return PooledObject;
}

void UObjectPoolFactory::OnReturnToPool(AActor* PoolActor)
{
	UPoolObjectActorComponent* PooledObject = CHelpers::GetComponent<UPoolObjectActorComponent>(PoolActor);
	if (PooledObject != nullptr)
	{
		PooledObject->SetActive(false);
		AvailableObjectPools[ObjectPools[PooledObject->GetPoolObjectID()].SpawnPoolIndex].Enqueue(PoolActor);
	}
}