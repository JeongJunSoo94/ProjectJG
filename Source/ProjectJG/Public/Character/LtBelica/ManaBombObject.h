#pragma once

#include "CoreMinimal.h"
#include "BaseSystem/BasePooledObject.h"
#include "ManaBombObject.generated.h"

UCLASS()
class PROJECTJG_API AManaBombObject : public ABasePooledObject
{
	GENERATED_BODY()
private:
    UPROPERTY(VisibleDefaultsOnly)
        class UStaticMeshComponent* Mesh;
    UPROPERTY(VisibleDefaultsOnly)
        class USphereComponent* Sphere;

    UPROPERTY(VisibleDefaultsOnly, Category = "ManaBomb")
        class UParticleSystem* InParticle;

    UPROPERTY(VisibleDefaultsOnly, Category = "ManaBomb")
        class UParticleSystem* LightTrailsParticle;

    UPROPERTY(VisibleDefaultsOnly, Category = "ManaBomb")
        class UParticleSystem* BurstParticle;

    UPROPERTY(VisibleDefaultsOnly, Category = "ManaBomb")
        class USoundCue* ManaBombSoundCue;
public:
    AManaBombObject();

protected:
    virtual void BeginPlay() override;
    virtual void Init() override;
   
    void Explosion();
    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
    virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
public: 
    void SetBombLocation(FVector location);
    void SetBombScale(float radius);
protected:
    FTimerHandle ExplosionTimer;
};
