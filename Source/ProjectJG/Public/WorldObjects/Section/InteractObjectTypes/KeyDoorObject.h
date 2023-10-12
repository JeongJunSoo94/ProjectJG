// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldObjects/Section/InteractObject.h"
#include "KeyDoorObject.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTJG_API AKeyDoorObject : public AInteractObject
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
		TArray<class ASpawnTrigger_Object*> SpawnTriggers;

	UPROPERTY(VisibleDefaultsOnly)
		class UBoxComponent* KeyDoor_BoxTrigger;

	UFUNCTION()
		void OnBeginOverlap_KeyDoor_BoxTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:
	AKeyDoorObject();

	virtual void BeginPlay() override;

	bool Check_Keys();

};
