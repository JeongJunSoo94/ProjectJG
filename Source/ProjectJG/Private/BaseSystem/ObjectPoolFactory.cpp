#include "BaseSystem/ObjectPoolFactory.h"
#include "Global.h"
#include "BaseSystem/BasePooledObject.h"

UObjectPoolFactory::UObjectPoolFactory()
{
}

void UObjectPoolFactory::BeginPlay()
{
	Super::BeginPlay();
}

void UObjectPoolFactory::CreateObject(UWorld* const World,int idx)
{
	ABasePooledObject* PooledObject = World->SpawnActor<ABasePooledObject>(PooledObjectSubclass, FVector::ZeroVector, FRotator::ZeroRotator);
	if (PooledObject != nullptr)
	{
		PooledObject->SetActive(false);
		PooledObject->SetPoolIndex(idx);
		PooledObject->OnReturnedToPool.BindUObject(this, &UObjectPoolFactory::OnReturnedToPool);
		ObjectPool.Add(PooledObject);
		AvailableObjectPool.Enqueue(PooledObject);
	}
}

void UObjectPoolFactory::Initialized()
{
	if (PooledObjectSubclass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			for (int i = 0; i < PoolSize; ++i)
			{
				CreateObject(World,i);
			}
		}
	}
}


ABasePooledObject* UObjectPoolFactory::SpawnObject()
{
	ABasePooledObject* PooledObject;
	if (AvailableObjectPool.IsEmpty())
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			CreateObject(World, ObjectPool.Num());
		}
	}
	AvailableObjectPool.Dequeue(PooledObject);
	return PooledObject;
}

void UObjectPoolFactory::OnReturnedToPool(ABasePooledObject* PoolActor)
{
	PoolActor->SetActive(false);
	AvailableObjectPool.Enqueue(PoolActor);
}