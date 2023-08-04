// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Bullet/CBullet.h"
#include "MurdockBullet.generated.h"

UCLASS()
class PROJECTJG_API AMurdockBullet : public ACBullet
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleDefaultsOnly)
		class UParticleSystem* BulletParticle;
public:
    AMurdockBullet();
    virtual UStaticMeshComponent* GetMesh() override { return Mesh; }

protected:
    virtual void BeginPlay() override;
    virtual void Init() override;
public:
    bool bInitailized;
};
