// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldObjects/Section/InteractObjectTypes/Abstract_Spawn_Object.h"
#include "Global.h"

void AAbstract_Spawn_Object::SpawnEnemy()
{
	for (ASpawnerActor* Spawner : SpawnerActors)
	{
		Spawner->StartSpawn();
	}
}

void AAbstract_Spawn_Object::StopSpawnEnemy()
{
	for (ASpawnerActor* Spawner : SpawnerActors)
	{
		Spawner->StopSpawn();
	}
}
