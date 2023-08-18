#pragma once

#include "CoreMinimal.h"
#include "Character/Components/CActionComponent.h"
#include "EAbliltyActionComponent.generated.h"

UCLASS()
class PROJECTJG_API UEAbliltyActionComponent : public UCActionComponent
{
    GENERATED_BODY()
private:
    UPROPERTY(VisibleDefaultsOnly, Category = "Ablilty")
        class UAnimMontage* EAbliltyMontage;

    UPROPERTY(VisibleDefaultsOnly, Category = "Eruption")
        class UParticleSystem* GroundPunchParticle;

    UPROPERTY(VisibleDefaultsOnly, Category = "Hologram")
        TSubclassOf<class AManaBombHologram> ManaBombHologramClass;
    UPROPERTY(VisibleDefaultsOnly, Category = "Hologram")
        class AManaBombHologram* ManaBombHologramActor;

    UPROPERTY(VisibleDefaultsOnly, Category = "ManaBomb")
        TSubclassOf<class AManaBombObject> ManaBombClass;
    UPROPERTY(VisibleDefaultsOnly)
        class UObjectPoolFactory* ObjectPoolFactory;
public:
    UEAbliltyActionComponent();
protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    virtual void OnAction() override;
    void HologramAction();
    void BeginAction()override;
    void EndAction()override;
    bool GetIsAbiliting() { return IsAbiliting; }
    void SetOwnerCharacter(ACBaseCharacter* character);
private:
    class ACBaseCharacter* OwnerCharacter;
private:
    bool IsAbiliting;

};