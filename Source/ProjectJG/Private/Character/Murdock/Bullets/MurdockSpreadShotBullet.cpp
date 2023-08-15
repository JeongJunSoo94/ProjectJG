// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Murdock/Bullets/MurdockSpreadShotBullet.h"
#include "Global.h"
#include "BaseSystem/BasePooledObject.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Materials/MaterialInstanceConstant.h"

AMurdockSpreadShotBullet::AMurdockSpreadShotBullet()
{
    CHelpers::CreateComponent(this, &Mesh, "Mesh");
    CHelpers::CreateActorComponent(this, &Projectile, "Projectile");

    UStaticMesh* mesh;
    CHelpers::GetAsset<UStaticMesh>(&mesh, "StaticMesh'/Game/Developers/GohyeongJu/Characters/Murdock/Bullets/SpreadShotMesh.SpreadShotMesh'");
    Mesh->SetStaticMesh(mesh);

    UMaterialInstanceConstant* material;
    CHelpers::GetAsset<UMaterialInstanceConstant>(&material, "MaterialInstanceConstant'/Game/Developers/GohyeongJu/Characters/Murdock/Bullets/M_Murdock_Bullet_Inst.M_Murdock_Bullet_Inst'");

    Mesh->SetMaterial(0, material);
    Mesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));

    Projectile->InitialSpeed = 2e+3f;
    Projectile->MaxSpeed = 2e+3f;
    Projectile->ProjectileGravityScale = 0.0f;
    Projectile->SetUpdatedComponent(Mesh);

    bInitailized = false;

    //SetActive(false);
}

void AMurdockSpreadShotBullet::Init()
{
    Projectile->Velocity = FQuat(GetActorRotation()).GetForwardVector() * Projectile->InitialSpeed;
    Projectile->SetUpdatedComponent(Mesh);

}

void AMurdockSpreadShotBullet::BeginPlay()
{
    Super::BeginPlay();
    Mesh->OnComponentHit.AddDynamic(this, &AMurdockSpreadShotBullet::OnHit);
}

void AMurdockSpreadShotBullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    HitTime = GetCurrentLifeTime();
    OnReturnedToPool.Execute(this);
    //Destroy();
}
