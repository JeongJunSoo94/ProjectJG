#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CActionComponent.generated.h"

DECLARE_DELEGATE_TwoParams(FOnUpdateTimer, float,float);

UCLASS(abstract)
class PROJECTJG_API UCActionComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    FOnUpdateTimer OnUpdateWidgetTimer;
public:
    UCActionComponent();
protected:
    UPROPERTY(EditAnywhere)
        class UTexture2D* WidgetTexture2D;
    UPROPERTY(VisibleDefaultsOnly)
        class UCharacterSkillWidget* CharacterSkillWidget;
protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
    virtual void SetOwnerCharacter(ACharacter* character);
    virtual void OnStartAction();
    virtual void OnEndAction();
    virtual void BeginNotifyAction();
    virtual void MiddleNotifyAction();
    virtual void EndNotifyAction();
    virtual void CoolTimeUpdate();
    void SetCharacterSkillWidget(UCharacterSkillWidget* SkillWidget) { CharacterSkillWidget= SkillWidget;}
    bool GetIsCoolTiming() { return IsCoolTiming; }
protected:
    bool IsCoolTiming = false;
    float CurCoolTime = 0.0f;
    float MaxCoolTime = 0.0f;
    FTimerHandle CoolTimeHandle;
};