// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldObjects/Section/InteractObjectTypes/Abstract_Spawn_Object.h"
#include "WorldObjects/Section/SectionEnd_Interface.h"
#include "SpawnTrigger_Object.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTJG_API ASpawnTrigger_Object : public AAbstract_Spawn_Object, public ISectionEnd_Interface
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleDefaultsOnly)
		class UBoxComponent* SpawnBoxTrigger;
public:

	ASpawnTrigger_Object();

	virtual void BeginPlay() override;
	UFUNCTION()
		virtual void PlayEndTrigger();

	UFUNCTION()
		void OnBeginOverlapSpawnBoxTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
