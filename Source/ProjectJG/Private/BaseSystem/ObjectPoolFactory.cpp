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

void UObjectPoolFactory::Initialized()
{
	if (PooledObjectSubclass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			for (int i = 0; i < PoolSize; ++i)
			{
				ABasePooledObject* PooledObject = World->SpawnActor<ABasePooledObject>(PooledObjectSubclass, FVector::ZeroVector, FRotator::ZeroRotator);
				if (PooledObject != nullptr)
				{
					PooledObject->SetActive(false);
					PooledObject->SetPoolIndex(i);
					PooledObject->OnReturnedToPool.BindUObject(this, &UObjectPoolFactory::OnReturnedToPool);
					ObjectPool.Add(PooledObject);
					AvailableObjectPool.Enqueue(PooledObject);
				}
			}
		}
	}
}

ABasePooledObject* UObjectPoolFactory::SpawnObject()
{
	ABasePooledObject* PooledObject;
	AvailableObjectPool.Dequeue(PooledObject);
	return PooledObject;
}

void UObjectPoolFactory::OnReturnedToPool(ABasePooledObject* PoolActor)
{
	PoolActor->SetActive(false);
	AvailableObjectPool.Enqueue(PoolActor);
}