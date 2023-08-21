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
public:
	URAbilityActionComponent();
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
