// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Character/Animation/CCharacterAnimInstance.h"
#include "RangeEnemyAnimInstance.generated.h"

UCLASS()
class PROJECTJG_API URangeEnemyAnimInstance : public UCCharacterAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	class ARangeEnemyCharacter* RangeEnemyCharacter;
};
