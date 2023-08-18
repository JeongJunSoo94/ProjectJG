#include "Character/LtBelica/EruptionObject.h"
#include "Global.h"
#include "BaseSystem/BasePooledObject.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "particles/PxParticleSystem.h"
#include "Particles/ParticleSystem.h"
#include "Character/Components/StatusComponent.h"
#include "Sound/SoundCue.h"

void AEruptionObject::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
}

AEruptionObject::AEruptionObject()
{
	CHelpers::CreateComponent<USphereComponent>(this, &Sphere, "Sphere");
	CHelpers::CreateActorComponent(this, &Projectile, "Projectile");
	//Sphere->bHiddenInGame = false;
	CHelpers::GetAsset<UParticleSystem>(&EruptionParticle, "ParticleSystem'/Game/ParagonLtBelica/FX/Particles/Belica/Abilities/EruptionBeam/FX/P_EruptionBeam2.P_EruptionBeam2'");

	CHelpers::GetAsset<USoundCue>(&EruptionSoundCue, "SoundCue'/Game/MilitaryWeapDark/Sound/GrenadeLauncher/GrenadeLauncher_Explosion_Cue.GrenadeLauncher_Explosion_Cue'");
}

void AEruptionObject::SetEruptionScale(float radius)
{
	Sphere->SetSphereRadius(radius);
}

float AEruptionObject::GetCollisionRadius()
{
	return Sphere->GetScaledSphereRadius();
}

void AEruptionObject::BeginPlay()
{

}

void AEruptionObject::Explosion()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EruptionParticle, GetActorLocation(), GetActorUpVector().Rotation(), true, EPSCPoolMethod::AutoRelease);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), EruptionSoundCue, GetActorLocation());
	for (auto actor : InteractionActor)
	{
	}
}

void AEruptionObject::Init()
{
	for (auto actor : InteractionActor)
	{
		actor.Value = false;
	}
	GetWorldTimerManager().SetTimer(ExplosionTimer, this, &AEruptionObject::Explosion, ActorLifeTime - 0.1f, false);
}

void AEruptionObject::NotifyActorBeginOverlap(AActor* OtherActor)
{
	UStatusComponent* Status = CHelpers::GetComponent<UStatusComponent>(OtherActor);
	CheckNull(Status);
	InteractionActor.Add(OtherActor,true);
}

void AEruptionObject::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (InteractionActor.Find(OtherActor))
	{
		InteractionActor[OtherActor] = false;
	}
}