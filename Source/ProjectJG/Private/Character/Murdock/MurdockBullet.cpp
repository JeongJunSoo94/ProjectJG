// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Murdock/MurdockBullet.h"
#include "Global.h"
#include "particles/PxParticleSystem.h"
#include "Particles/ParticleSystem.h"
#include "BaseSystem/BasePooledObject.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Components/SphereComponent.h"

AMurdockBullet::AMurdockBullet()
{
    //CHelpers::CreateComponent(this, &Mesh, "Mesh");
    //CHelpers::CreateActorComponent(this, &Projectile, "Projectile");

    //UStaticMesh* mesh;
    //CHelpers::GetAsset<UStaticMesh>(&mesh, "StaticMesh'/Game/Developers/USER/Bullet/Sphere.Sphere'");
    //Mesh->SetStaticMesh(mesh);

    //UMaterialInstanceConstant* material;
    //CHelpers::GetAsset<UMaterialInstanceConstant>(&material, "MaterialInstanceConstant'/Game/Developers/USER/Bullet/M_Belica_Projectile_Mesh_Inst.M_Belica_Projectile_Mesh_Inst'");

    //Mesh->SetMaterial(0, material);

    //Projectile->InitialSpeed = 2e+4f;
    //Projectile->MaxSpeed = 2e+4f;
    //Projectile->ProjectileGravityScale = 0.0f;
    //Projectile->SetUpdatedComponent(Mesh);
  
    //CHelpers::CreateComponent<USphereComponent>(this,&RootSphere,"RootSphere");

    Mesh->SetRelativeScale3D(FVector(0.1f, 0.3f, 0.1f));
    Mesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
    bInitailized = false;

    CHelpers::GetAsset<UParticleSystem>(&BulletParticle, "ParticleSystem'/Game/ParagonMurdock/FX/Particles/Abilities/Primary/FX/P_PlasmaShot_Projectile_Trail_2.P_PlasmaShot_Projectile_Trail_2'");


}

void AMurdockBullet::BeginPlay()
{
    Super::BeginPlay();
    
}

void AMurdockBullet::Init()
{
    Super::Init();
    FVector forwardVector(200.0f, 0.0f, 0.0f);
    UGameplayStatics::SpawnEmitterAttached(BulletParticle, GetMesh(), "Mesh", forwardVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true, EPSCPoolMethod::AutoRelease);

}
