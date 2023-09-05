#pragma once

#include "CoreMinimal.h"
#include "Character/Components/CActionComponent.h"
#include "CQAbliltyActionComponent.generated.h"

UCLASS()
class PROJECTJG_API UCQAbliltyActionComponent : public UCActionComponent
{
	GENERATED_BODY()
private:
    UPROPERTY(VisibleDefaultsOnly, Category = "Ablilty")
        class UAnimMontage* QAbliltyMontage;

    UPROPERTY(VisibleDefaultsOnly, Category = "Eruption")
        class UParticleSystem* GroundPunchParticle;

    UPROPERTY(VisibleDefaultsOnly, Category = "Eruption")
        class UParticleSystem* EruptionBeamParticle;

    UPROPERTY(VisibleDefaultsOnly, Category = "Eruption")
        class UParticleSystem* EruptionRocksParticle;

    UPROPERTY(VisibleDefaultsOnly, Category = "Eruption")
        class UParticleSystem* HandParticle;
    UPROPERTY(VisibleDefaultsOnly, Category = "Eruption")
        class UParticleSystem* HologramParticle;

    UPROPERTY(VisibleDefaultsOnly, Category = "Eruption")
        class UParticleSystemComponent* HandParticleComponent;

    UPROPERTY(VisibleDefaultsOnly, Category = "Hologram")
        TSubclassOf<class AEruptionHologram> EruptionHologramClass;
    UPROPERTY(VisibleDefaultsOnly, Category = "Hologram")
        class AEruptionHologram* EruptionHologramActor;

    UPROPERTY(VisibleDefaultsOnly, Category = "Eruption")
        TSubclassOf<class AEruptionObject> EruptionClass;
    UPROPERTY(VisibleDefaultsOnly)
        class UObjectPoolFactory* ObjectPoolFactory;
public:
    UCQAbliltyActionComponent();
protected:
    virtual void BeginPlay() override;
protected:
    class ACBaseCharacter* OwnerCharacter;
public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    virtual void SetOwnerCharacter(ACharacter* character) override;
    virtual void OnStartAction() override;
    void HologramAction();
    virtual void BeginNotifyAction()override;
    virtual void EndNotifyAction()override;
    bool GetIsAbiliting() { return IsAbiliting; }
private:
    FVector AbilityDirection;
    bool IsAbiliting;
};
