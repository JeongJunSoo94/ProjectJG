#pragma once

#include "CoreMinimal.h"
#include "Character/Components/CActionComponent.h"
#include "RAbilityActionComponent.generated.h"

UCLASS()
class PROJECTJG_API URAbilityActionComponent : public UCActionComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Ablilty")
		class UAnimMontage* RAbliltyMontage;
    UPROPERTY(VisibleDefaultsOnly, Category = "Pistol")
        TSubclassOf<class ACBullet> BulletClass;
    UPROPERTY(VisibleDefaultsOnly, Category = "Pistol")
        class UMaterialInstanceConstant* DecalMaterial;
    UPROPERTY(VisibleDefaultsOnly, Category = "Pistol")
        int32 MuzzleIndex;
    UPROPERTY(VisibleDefaultsOnly, Category = "Pistol")
        class UParticleSystem* ImpactParticle;
    UFUNCTION()
        void OnHitPaticle(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    UPROPERTY(VisibleDefaultsOnly)
        class UObjectPoolFactory* ObjectPoolFactory;
public:
	URAbilityActionComponent();
private:
    class ACBaseCharacter* OwnerCharacter;
protected:
	virtual void BeginPlay() override;
public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    void SetOwnerCharacter(ACBaseCharacter* character);
    virtual void OnAction() override;
    void HologramAction();
    void BeginAction()override;
    void EndAction()override;
    bool GetIsAbiliting() { return IsAbiliting; }
private:
    bool IsAbiliting;
};
