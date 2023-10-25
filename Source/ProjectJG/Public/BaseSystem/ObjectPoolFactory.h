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
	
	TMap<FName, FActorArray> ObjectPools; // 오브젝트 이름 , 전체 오브젝트를 관리..

	TArray<TQueue<AActor*>> AvailableObjectPools; //선입 선출을 위해 오브젝트들을 관리.. ObjectPools 안에있는 actor들 중 true인 녀석들만 가지고있음.
	//TMap<FName, FActorQueue> AvailableObjectPools;
	TMap<FName, int> SpawnPoolIndexs;

};
