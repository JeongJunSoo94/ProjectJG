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
    UPROPERTY(VisibleDefaultsOnly, Category = "ManaBoom")
        class UParticleSystem* ImpactParticle;
public:
    UCQAbliltyActionComponent();
protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    virtual void OnAction() override;
    void BeginAction()override;
    void EndAction()override;
    bool GetIsAbiliting() { return IsAbiliting; }
private:
    bool IsAbiliting;
};
