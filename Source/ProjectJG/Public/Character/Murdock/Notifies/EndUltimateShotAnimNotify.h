// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "EndUltimateShotAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTJG_API UEndUltimateShotAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleDefaultsOnly)
		class AMurdock* cachingMurdock;
public:
	FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
