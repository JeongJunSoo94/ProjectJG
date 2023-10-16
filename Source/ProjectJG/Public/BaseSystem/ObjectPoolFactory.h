// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObjectPoolFactory.generated.h"

USTRUCT(BlueprintType)
struct FActorArray
{
	GENERATED_USTRUCT_BODY()
public:
	int SpawnPoolIndex=0;
	TArray<AActor*> actorArray;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTJG_API UObjectPoolFactory : public UActorComponent
{
	GENERATED_BODY()

public:	
	UObjectPoolFactory();
	
	UFUNCTION()
		void OnReturnToPool(AActor* PoolActor);
	AActor* SpawnObject(TSubclassOf<class AActor> PooledObjectSubclass);
	void CreateObject(unsigned int createCount, TSubclassOf<class AActor> pooledObjectSubclass);
protected:
	virtual void BeginPlay() override;
protected:
	TMap<FName, FActorArray> ObjectPools;
	TArray<TQueue<AActor*>> AvailableObjectPools;

};
