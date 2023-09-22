#include "BaseSystem/PoolObjectActorComponent.h"
#include "Global.h"
#include "BaseSystem/ObjectPoolFunctions.h"

UPoolObjectActorComponent::UPoolObjectActorComponent()
{
}

void UPoolObjectActorComponent::BeginPlay()
{
}

void UPoolObjectActorComponent::SetActive(bool IsActive)
{
	bPooledObjectActive = IsActive;
	GetOwner()->SetActorHiddenInGame(!bPooledObjectActive);
	GetOwner()->SetActorEnableCollision(bPooledObjectActive);
	GetOwner()->SetActorTickEnabled(bPooledObjectActive);
	if (bPooledObjectActive)
	{
		GetWorld()->GetTimerManager().SetTimer(LifeTimer, this, &UPoolObjectActorComponent::Deactive, ActorLifeTime, false);
		IObjectPoolFunctions* ObjectPool = Cast<IObjectPoolFunctions>(GetOwner());
		CheckNull(ObjectPool);
		ObjectPool->Init();
	}
	else
	{
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	}
}

void UPoolObjectActorComponent::Deactive()
{
	OnReturnToPool.Execute(GetOwner());
}

void UPoolObjectActorComponent::SetPoolIndex(int Index, FName ObjectID)
{
	PoolObjectIndex = Index;
	PoolObjectID = ObjectID;
}

void UPoolObjectActorComponent::SetActorLifeTime(float LifeTime)
{
	ActorLifeTime = LifeTime;
}

bool UPoolObjectActorComponent::IsActive()
{
	return bPooledObjectActive;
}

int UPoolObjectActorComponent::GetPoolIndex()
{
	return PoolObjectIndex;
}

void UPoolObjectActorComponent::Init()
{
}

void UPoolObjectActorComponent ::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
}
float UPoolObjectActorComponent::GetCurrentLifeTime()
{
	return GetWorld()->GetTimerManager().GetTimerElapsed(LifeTimer);
}

