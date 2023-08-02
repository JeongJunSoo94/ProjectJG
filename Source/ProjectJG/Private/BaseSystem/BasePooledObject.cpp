#include "BaseSystem/BasePooledObject.h"
#include "Global.h"

ABasePooledObject::ABasePooledObject()
{
}

void ABasePooledObject::SetActive(bool IsActive)
{
	bPooledObjectActive = IsActive;
	SetActorHiddenInGame(!IsActive);
	SetActorEnableCollision(bPooledObjectActive);
	SetActorTickEnabled(bPooledObjectActive);
	if (IsActive)
	{
		GetWorldTimerManager().SetTimer(LifeTimer, this, &ABasePooledObject::Deactive, ActorLifeTime, false);
		Init();
	}
	else
	{
		Clog::Log("end");
		GetWorldTimerManager().ClearAllTimersForObject(this);
	}
}

void ABasePooledObject::Deactive()
{
	OnReturnedToPool.Execute(this);
}

void ABasePooledObject::SetPoolIndex(int Index)
{
	PoolObjectIndex=Index;
}

void ABasePooledObject::SetActorLifeTime(float LifeTime)
{
	ActorLifeTime = LifeTime;
}

bool ABasePooledObject::IsActive()
{
	return bPooledObjectActive;
}

int ABasePooledObject::GetPoolIndex()
{
	return PoolObjectIndex;
}

void ABasePooledObject::BeginPlay()
{
	Super::BeginPlay();
}

void ABasePooledObject::Init()
{
}

void ABasePooledObject::Tick(float DeltaTime)
{
}

