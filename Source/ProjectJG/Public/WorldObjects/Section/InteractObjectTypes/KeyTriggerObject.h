// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldObjects/Section/InteractObjectTypes/Abstract_Spawn_Object.h"
#include "KeyTriggerObject.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTJG_API AKeyTriggerObject : public AAbstract_Spawn_Object
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
		TArray<class AStaticMeshActor*> KeyObjects;
		TArray<class UMaterialInstanceDynamic*> MaterialInstanceDynamic_KeyObjects;
	UPROPERTY(VisibleDefaultsOnly)
		class UBoxComponent* Key_BoxTrigger;
	UPROPERTY(EditAnywhere)
		uint8 KeyNumber;
	UFUNCTION()
		void OnBeginOverlap_KeyBoxTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	AKeyTriggerObject();
	virtual void BeginPlay() override;


	void takebyPlayer(class ACBaseCharacter* Player);

	void SetUpLightMaterial();
	void OnLight_Object();
	void OffLight_Object();
};
