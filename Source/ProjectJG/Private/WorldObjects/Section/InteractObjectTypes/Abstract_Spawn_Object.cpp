#include "WorldObjects/Section/InteractObjectTypes/Abstract_Spawn_Object.h"
#include "Global.h"
#include "Spawner/SpawnerActor.h"

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
