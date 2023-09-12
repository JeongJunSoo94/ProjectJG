#pragma once

#include "CoreMinimal.h"
#include "BaseSystem/BasePooledObject.h"
#include "EruptionObject.generated.h"

UCLASS()
class PROJECTJG_API AEruptionObject : public ABasePooledObject
{
	GENERATED_BODY()
private:

    UPROPERTY(VisibleDefaultsOnly)
        class USphereComponent* Sphere;
    UPROPERTY(VisibleDefaultsOnly, Category = "Eruption")
        class UParticleSystem* EruptionParticle;
    UPROPERTY(VisibleDefaultsOnly)
        class UProjectileMovementComponent* Projectile;
    UFUNCTION()
        void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
            UPrimitiveComponent* OtherComp, FVector  NormalImpulse,
            const FHitResult& Hit);

    UPROPERTY(VisibleDefaultsOnly, Category = "Eruption")
        class USoundCue* EruptionSoundCue;

public:
    AEruptionObject();
    void SetEruptionScale(float radius);
    float GetCollisionRadius();
protected:
    virtual void BeginPlay() override;
    virtual void Init()override;
    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
    virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
    void Explosion();

public:
    bool bInitailized;
protected:
    TMap<AActor*,bool> InteractionActors;
    FTimerHandle ExplosionTimer;
};
