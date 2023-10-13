// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldObjects/Section/InteractObjectTypes/KeyTriggerObject.h"
#include "Global.h"
#include "Components/BoxComponent.h"
#include "Character/CBaseCharacter.h"

#include "BaseSystem/GameStateBase/InGameStateBase.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/StaticMeshActor.h"

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
	OffLight_Object();
}


void AKeyTriggerObject::BeginPlay()
{
	//Super::BeginPlay();

	CHelpers::CheckNullComponent<UBoxComponent>(this, &Key_BoxTrigger);

	Key_BoxTrigger->OnComponentBeginOverlap.AddDynamic(this, &AKeyTriggerObject::OnBeginOverlap_KeyBoxTrigger);

	SetUpLightMaterial();
	OnLight_Object();
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

void AKeyTriggerObject::SetUpLightMaterial()
{
	int32 arrayNum = 0;
	for (AStaticMeshActor* object : KeyObjects)
	{
		UMaterialInstanceDynamic* materialDynamic = object->GetStaticMeshComponent()->CreateDynamicMaterialInstance(arrayNum);
		++arrayNum;
		MaterialInstanceDynamic_KeyObjects.Add(materialDynamic);
	}
}

void AKeyTriggerObject::OnLight_Object()
{
	for (UMaterialInstanceDynamic* material : MaterialInstanceDynamic_KeyObjects)
	{
		material->SetScalarParameterValue(TEXT("IsGlow"), 1.0f);
	}
}

void AKeyTriggerObject::OffLight_Object()
{
	for (UMaterialInstanceDynamic* material : MaterialInstanceDynamic_KeyObjects)
	{
		material->SetScalarParameterValue(TEXT("IsGlow"), -1.0f);
	}
}