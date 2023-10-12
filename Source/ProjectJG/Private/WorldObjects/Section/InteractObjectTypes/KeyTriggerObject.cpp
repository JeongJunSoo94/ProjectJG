// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldObjects/Section/InteractObjectTypes/KeyTriggerObject.h"
#include "Global.h"
#include "Components/BoxComponent.h"
#include "Character/CBaseCharacter.h"

#include "BaseSystem/GameStateBase/InGameStateBase.h"


AKeyTriggerObject::AKeyTriggerObject()
{
	CHelpers::CreateComponent<UBoxComponent>(this, &Key_BoxTrigger, "Key_BoxTrigger");
	Key_BoxTrigger->bHiddenInGame = false;
}

void AKeyTriggerObject::OnBeginOverlap_KeyBoxTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	ACBaseCharacter* player = Cast<ACBaseCharacter>(OtherActor);
	if (!player)
	{
		Clog::Log("player is Null");
		return;
	}
	takebyPlayer(player);
	SpawnEnemy();
}


void AKeyTriggerObject::BeginPlay()
{
	//Super::BeginPlay();

	CHelpers::CheckNullComponent<UBoxComponent>(this, &Key_BoxTrigger);

	Key_BoxTrigger->OnComponentBeginOverlap.AddDynamic(this, &AKeyTriggerObject::OnBeginOverlap_KeyBoxTrigger);
}

void AKeyTriggerObject::takebyPlayer(ACBaseCharacter* Player)
{
	AInGameStateBase* GameState = Cast<AInGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	if (!GameState)
	{
		Clog::Log("GameState is Null");
		return;
	}
	GameState->TakeKey(KeyNumber);

}
