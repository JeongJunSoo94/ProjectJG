#pragma once

#include "CoreMinimal.h"
#include "Character/Components/CActionComponent.h"
#include "MeleeAttackActionComponent.generated.h"

UCLASS()
class PROJECTJG_API UMeleeAttackActionComponent : public UCActionComponent
{
	GENERATED_BODY()
private:
    UPROPERTY(VisibleDefaultsOnly, Category = "Melee")
        class UAnimInstance* ABPInstance;
    UPROPERTY(VisibleDefaultsOnly, Category = "Melee")
        class UAnimMontage* MeleeMontage;
private:
    class AMeleeEnemyCharacter* OwnerCharacter;
public:
    UMeleeAttackActionComponent();
    virtual void SetOwnerCharacter(ACharacter* character) override;
    virtual void MiddleNotifyAction() override;
    virtual void OnStartAction() override;
    virtual void BeginNotifyAction()override;
    virtual void EndNotifyAction()override;
};
