// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSystem/BasePooledObject.h"
#include "MurdockSpreadShotBullet.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTJG_API AMurdockSpreadShotBullet : public ABasePooledObject
{
	GENERATED_BODY()
protected:
    UPROPERTY(VisibleDefaultsOnly)
        class UStaticMeshComponent* Mesh;
    UPROPERTY(VisibleDefaultsOnly)
        class UProjectileMovementComponent* Projectile;
    UFUNCTION()
        void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
            UPrimitiveComponent* OtherComp, FVector  NormalImpulse,
            const FHitResult& Hit);
public:
    AMurdockSpreadShotBullet();
    virtual UStaticMeshComponent* GetMesh() { return Mesh; }

protected:
    virtual void BeginPlay() override;
    virtual void Init()override;
    float HitTime;
public:
    float GetHitTime() { return HitTime; }
    bool bInitailized;
};
