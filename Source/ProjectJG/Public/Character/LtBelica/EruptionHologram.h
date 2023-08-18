#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EruptionHologram.generated.h"

UCLASS()
class PROJECTJG_API AEruptionHologram : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleDefaultsOnly)
		class UBoxComponent* Box;
	UPROPERTY(VisibleDefaultsOnly, Category = "Eruption")
		class UDecalComponent* Decal;
	UPROPERTY(VisibleDefaultsOnly, Category = "Eruption")
		class UMaterialInstanceConstant* DecalMaterial;
public:	
	AEruptionHologram();

protected:
	virtual void BeginPlay() override;


public:	
	virtual void Tick(float DeltaTime) override;
	void SetHologramLocation(FVector location);
	void SetHologramRotator(FRotator rotator);
	void SetHologramScale(FVector scale);
	void SetActive(bool isActive);
	FVector GetHologramBoxScale();
	FVector GetHologramDecalScale();
};
