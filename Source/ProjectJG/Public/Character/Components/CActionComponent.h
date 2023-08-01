#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CActionComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTJG_API UCActionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UCActionComponent();

protected:
    virtual void BeginPlay() override;
protected:
    class ACBaseCharacter* OwnerCharacter;
public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:
    void SetOwnerCharacter(ACBaseCharacter* character);
    virtual void OnAction();
    virtual void BeginAction();
    virtual void EndAction();
};