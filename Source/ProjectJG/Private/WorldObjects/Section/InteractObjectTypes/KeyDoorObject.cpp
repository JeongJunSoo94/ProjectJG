// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldObjects/Section/InteractObjectTypes/KeyDoorObject.h"
#include "Global.h"

#include "WorldObjects/Section/InteractObjectTypes/SpawnTrigger_Object.h"
#include "BaseSystem/GameStateBase/InGameStateBase.h"
#include "Components/BoxComponent.h"
#include "BaseSystem/GameStateBase/InGameStateBase.h"



void AKeyDoorObject::OnBeginOverlap_KeyDoor_BoxTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	ACBaseCharacter* player = Cast<ACBaseCharacter>(OtherActor);
	if (Check_Keys()&& !!player)
	{
		for (ASpawnTrigger_Object* SpawnTrigger : SpawnTriggers)
		{
			SpawnTrigger->StopSpawnEnemy();
		}
		PlaySequence(ETriggerType::End);
		
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("Title"));

	}
}

AKeyDoorObject::AKeyDoorObject()
{
	CHelpers::CreateComponent<UBoxComponent>(this, &KeyDoor_BoxTrigger, "SectionStart_BoxTrigger");
	KeyDoor_BoxTrigger->bHiddenInGame = false;
}

void AKeyDoorObject::BeginPlay()
{
	KeyDoor_BoxTrigger->OnComponentBeginOverlap.AddDynamic(this, &AKeyDoorObject::OnBeginOverlap_KeyDoor_BoxTrigger);

}

bool AKeyDoorObject::Check_Keys()
{
	AInGameStateBase* GameState = Cast<AInGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));

	return GameState->GetClearCondition()->IsGoal();
}
