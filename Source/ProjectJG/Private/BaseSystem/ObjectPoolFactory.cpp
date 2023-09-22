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
				SpawnPoolIndexs.Add(pooledObjectSubclass->GetFName(), SpawnPoolIndexs.Num());
				AvailableObjectPools.SetNum(AvailableObjectPools.Num() + 1);
			}
			PooledObject->SetPoolIndex(ObjectPools[pooledObjectSubclass->GetFName()].actorArray.Num(), pooledObjectSubclass->GetFName());
			PooledObject->OnReturnToPool.BindUObject(this, &UObjectPoolFactory::OnReturnToPool);
			ObjectPools[pooledObjectSubclass->GetFName()].actorArray.Add(actor);
			ObjectPools[pooledObjectSubclass->GetFName()].actorIsActive.Add(false);
			
			AvailableObjectPools[SpawnPoolIndexs[pooledObjectSubclass->GetFName()]].Enqueue(actor);
			//AvailableObjectPools[SpawnPoolIndexs[pooledObjectSubclass->GetFName()]].actorQueue.Enqueue(actor);

			//if (AvailableObjectPools.Find(pooledObjectSubclass->GetFName()) == nullptr)
			//{
			//	//AvailableObjectPools.Add(pooledObjectSubclass->GetFName());// [pooledObjectSubclass->GetFName()] ;
			//}
			//AvailableObjectPools[pooledObjectSubclass->GetFName()].actorQueue.Enqueue(actor);
		}
	}
}

AActor* UObjectPoolFactory::SpawnObject(TSubclassOf<class AActor> pooledObjectSubclass)
{
	AActor* PooledObject=nullptr;
	//for (int i = 0; i < ObjectPools[pooledObjectSubclass->GetFName()].actorArray.Num(); ++i)
	//{
	//	if (ObjectPools[pooledObjectSubclass->GetFName()].actorIsActive[i])
	//	{
	//		PooledObject = ObjectPools[pooledObjectSubclass->GetFName()].actorArray[i];
	//		ObjectPools[pooledObjectSubclass->GetFName()].actorIsActive[i] = true;
	//		return PooledObject;
	//	}
	//}
	//CreateObject(1, pooledObjectSubclass);
	//PooledObject = ObjectPools[pooledObjectSubclass->GetFName()].actorArray[ObjectPools[pooledObjectSubclass->GetFName()].actorArray.Num()-1];
	//ObjectPools[pooledObjectSubclass->GetFName()].actorIsActive[ObjectPools[pooledObjectSubclass->GetFName()].actorArray.Num() - 1] = true;

	if (AvailableObjectPools[SpawnPoolIndexs[pooledObjectSubclass->GetFName()]].IsEmpty())
	{
		CreateObject(1, pooledObjectSubclass);
	}
	AvailableObjectPools[SpawnPoolIndexs[pooledObjectSubclass->GetFName()]].Dequeue(PooledObject);
	return PooledObject;
}

void UObjectPoolFactory::OnReturnToPool(AActor* PoolActor)
{
	UPoolObjectActorComponent* PooledObject = CHelpers::GetComponent<UPoolObjectActorComponent>(PoolActor);
	if (PooledObject != nullptr)
	{
		PooledObject->SetActive(false);
		//ObjectPools[PooledObject->GetPoolObjectID()].actorIsActive[PooledObject->GetPoolIndex()]=false;
		AvailableObjectPools[SpawnPoolIndexs[PooledObject->GetPoolObjectID()]].Enqueue(PoolActor);
	}
}