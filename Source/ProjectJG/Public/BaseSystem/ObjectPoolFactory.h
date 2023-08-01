// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObjectPoolFactory.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTJG_API UObjectPoolFactory : public UActorComponent
{
	GENERATED_BODY()

public:	
	UObjectPoolFactory();
	
	UFUNCTION(BlueprintCallable, Category = "ObjectPool")
		ABasePooledObject* SpawnObject();

	UPROPERTY(EditAnywhere, Category = "ObjectPool")
		int PoolSize;
	UPROPERTY(EditAnywhere, Category = "ObjectPool")
		TSubclassOf<class ABasePooledObject> PooledObjectSubclass;

	UFUNCTION()
		void OnReturnedToPool(ABasePooledObject* PoolActor);
	UFUNCTION()
		void Initialized();
protected:
	virtual void BeginPlay() override;

	TArray<ABasePooledObject*> ObjectPool;
	TQueue<ABasePooledObject*> AvailableObjectPool;
	TArray<int> SpawnPoolIndexs;


		
};
