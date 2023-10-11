// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldObjects/Section/InteractObject.h"
#include "Abstract_Spawn_Object.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class PROJECTJG_API AAbstract_Spawn_Object : public AInteractObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "SpawnerActors")
		TArray<class ASpawnerActor*> SpawnerActors;

	virtual void SpawnEnemy();
	virtual void StopSpawnEnemy();
};
