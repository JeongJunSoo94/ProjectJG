#include "Character/LtBelica/ManaBombObject.h"
#include "Global.h"
#include "BaseSystem/BasePooledObject.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "particles/PxParticleSystem.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"

AManaBombObject::AManaBombObject()
{
    CHelpers::CreateComponent<USphereComponent>(this, &Sphere, "Sphere");
    CHelpers::CreateComponent(this, &Mesh, "Mesh", Sphere);

    Sphere->bHiddenInGame = false;

    UStaticMesh* mesh;
    CHelpers::GetAsset<UStaticMesh>(&mesh, "StaticMesh'/Game/Developers/JJS/LtBelica/FX/Meshes/SphereManaBomb.SphereManaBomb'");
    Mesh->SetStaticMesh(mesh);

    UMaterialInstanceConstant* material;
    CHelpers::GetAsset<UMaterialInstanceConstant>(&material, "MaterialInstanceConstant'/Game/Developers/JJS/LtBelica/FX/Materials/ManaBomb/M_Electricity_MASTER_Inst.M_Electricity_MASTER_Inst'");

    Mesh->SetMaterial(0, material);

    CHelpers::GetAsset<UParticleSystem>(&InParticle, "ParticleSystem'/Game/ParagonLtBelica/FX/Particles/Belica/Abilities/ManaBomb/FX/P_BombBeamIn.P_BombBeamIn'");
    CHelpers::GetAsset<UParticleSystem>(&LightTrailsParticle, "ParticleSystem'/Game/ParagonLtBelica/FX/Particles/Belica/Abilities/ManaBomb/FX/P_BombLightTrails.P_BombLightTrails'");
    CHelpers::GetAsset<UParticleSystem>(&BurstParticle, "ParticleSystem'/Game/ParagonLtBelica/FX/Particles/Belica/Abilities/ManaBomb/FX/P_ManaBombBurst.P_ManaBombBurst'");

    CHelpers::GetAsset<USoundCue>(&ManaBombSoundCue, "SoundCue'/Game/MilitaryWeapDark/Sound/GrenadeLauncher/GrenadeLauncher_Explosion_Cue.GrenadeLauncher_Explosion_Cue'");
}

void AManaBombObject::BeginPlay()
{
}

void AManaBombObject::Init()
{
    GetWorldTimerManager().SetTimer(ExplosionTimer, this, &AManaBombObject::Explosion, ActorLifeTime-0.1f, false);
}

void AManaBombObject::SetBombLocation(FVector location)
{
    SetActorLocation(location);
    Mesh->UpdateComponentToWorld();
    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), InParticle, GetActorLocation(), GetActorRotation(), true, EPSCPoolMethod::AutoRelease);
    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LightTrailsParticle, GetActorLocation(), GetActorRotation(), true, EPSCPoolMethod::AutoRelease);
}

void AManaBombObject::SetBombScale(float radius)
{
    Sphere->SetSphereRadius(radius);
}

void AManaBombObject::Explosion()
{
    Mesh->Activate(false);
    Mesh->UpdateComponentToWorld();
    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BurstParticle, GetActorLocation(), GetActorRotation(), true, EPSCPoolMethod::AutoRelease);
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), ManaBombSoundCue, GetActorLocation());
}

void AManaBombObject::NotifyActorBeginOverlap(AActor* OtherActor)
{

}

void AManaBombObject::NotifyActorEndOverlap(AActor* OtherActor)
{
}

