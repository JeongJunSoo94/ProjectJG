// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSpawnPoint.generated.h"

UCLASS()
class PROJECTJG_API AItemSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemSpawnPoint();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class AItem>> ItemClasses;

	UPROPERTY()
	AItem* SpawnedItem;

	void SpawnItem();
	void SpawnItemTimerFinished();

	UFUNCTION()
	void StartSpawnItemTimer(AActor* DestroyedActor);
private:
	FTimerHandle SpawnItemTimer;

	UPROPERTY(EditAnywhere)
	float SpawnItemTimeMin;

	UPROPERTY(EditAnywhere)
	float SpawnItemTimeMax;

};
