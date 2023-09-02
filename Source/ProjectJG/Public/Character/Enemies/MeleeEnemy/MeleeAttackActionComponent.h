#pragma once

#include "CoreMinimal.h"
#include "Character/Components/CActionComponent.h"
#include "MeleeAttackActionComponent.generated.h"

UCLASS()
class PROJECTJG_API UMeleeAttackActionComponent : public UCActionComponent
{
	GENERATED_BODY()
private:
    class AMeleeEnemyCharacter* OwnerCharacter;
public:
    UMeleeAttackActionComponent();
    virtual void SetOwnerCharacter(ACharacter* character) override;
    virtual void OnAction() override;
    virtual void BeginAction()override;
    virtual void EndAction()override;
};
