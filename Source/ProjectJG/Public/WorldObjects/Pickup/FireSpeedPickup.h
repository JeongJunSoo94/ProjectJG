// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldObjects/Pickup/Pickup.h"
#include "FireSpeedPickup.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTJG_API AFireSpeedPickup : public APickup
{
	GENERATED_BODY()
protected:
	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
private:

	UPROPERTY(EditAnywhere)
	float BaseSpeedBuff = 0.5f;

	UPROPERTY(EditAnywhere)
	float SpeedBuffTime = 30.f;
};
