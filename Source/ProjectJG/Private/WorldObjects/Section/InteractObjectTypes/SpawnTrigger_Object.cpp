// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldObjects/Section/InteractObjectTypes/SpawnTrigger_Object.h"
#include "Global.h"
#include "Components/BoxComponent.h"

ASpawnTrigger_Object::ASpawnTrigger_Object()
{
	CHelpers::CreateComponent<UBoxComponent>(this, &SpawnBoxTrigger, "SpawnBoxTrigger");
	//SpawnBoxTrigger->bHiddenInGame = false;
}

void ASpawnTrigger_Object::OnBeginOverlapSpawnBoxTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SpawnEnemy();
}

void ASpawnTrigger_Object::BeginPlay()
{
	Super::BeginPlay();

	SpawnBoxTrigger->OnComponentBeginOverlap.AddDynamic(this, &ASpawnTrigger_Object::OnBeginOverlapSpawnBoxTrigger);
}

void ASpawnTrigger_Object::PlayEndTrigger()
{
	StopSpawnEnemy();
}

