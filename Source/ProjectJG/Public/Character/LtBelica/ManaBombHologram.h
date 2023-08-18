#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ManaBombHologram.generated.h"

UCLASS()
class PROJECTJG_API AManaBombHologram : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, Category = "ManaBom")
		class UDecalComponent* Decal;
	UPROPERTY(VisibleDefaultsOnly, Category = "ManaBom")
		class UMaterialInstanceConstant* DecalMaterial;
public:	
	AManaBombHologram();
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	void SetHologramLocation(FVector location);
	void SetHologramScale(FVector scale);
	void SetActive(bool isActive);
protected:
	float ActorLifeTime = 0.0f;
	FTimerHandle LifeTimer;
};
