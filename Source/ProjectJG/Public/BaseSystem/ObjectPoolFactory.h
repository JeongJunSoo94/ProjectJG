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
	TArray<AActor*> actorArray;
	TArray<bool> actorIsActive;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTJG_API UObjectPoolFactory : public UActorComponent
{
	GENERATED_BODY()

public:	
	UObjectPoolFactory();
	
	UFUNCTION(BlueprintCallable, Category = "ObjectPool")
		AActor* SpawnObject(TSubclassOf<class AActor> PooledObjectSubclass);
	UFUNCTION()
		void OnReturnToPool(AActor* PoolActor);
	UFUNCTION()
		void CreateObject(unsigned int createCount, TSubclassOf<class AActor> pooledObjectSubclass);
protected:
	virtual void BeginPlay() override;
	
	TMap<FName, FActorArray> ObjectPools;

	TArray<TQueue<AActor*>> AvailableObjectPools;
	//TMap<FName, FActorQueue> AvailableObjectPools;
	TMap<FName, int> SpawnPoolIndexs;

};
