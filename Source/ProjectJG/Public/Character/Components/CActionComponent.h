#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CActionComponent.generated.h"


UCLASS(abstract)
class PROJECTJG_API UCActionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UCActionComponent();

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
    virtual void SetOwnerCharacter(ACharacter* character);
    virtual void OnStartAction();
    virtual void BeginNotifyAction();
    virtual void MiddleNotifyAction();
    virtual void EndNotifyAction();
};