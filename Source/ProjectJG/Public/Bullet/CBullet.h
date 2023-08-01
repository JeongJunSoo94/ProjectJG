#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseSystem/BasePooledObject.h"
#include "CBullet.generated.h"

UCLASS()
class PROJECTJG_API ACBullet : public ABasePooledObject
{
    GENERATED_BODY()
private:
    UPROPERTY(VisibleDefaultsOnly)
        class UStaticMeshComponent* Mesh;
    UPROPERTY(VisibleDefaultsOnly)
        class UProjectileMovementComponent* Projectile;
    UFUNCTION()
        void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
            UPrimitiveComponent* OtherComp, FVector  NormalImpulse,
            const FHitResult& Hit);
public:
    ACBullet();
    UStaticMeshComponent* GetMesh() { return Mesh; }

protected:
    virtual void BeginPlay() override;
    virtual void Init()override;
public:
    bool bInitailized;
};