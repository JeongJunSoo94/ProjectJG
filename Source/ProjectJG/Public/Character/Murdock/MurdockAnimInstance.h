// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Animation/CCharacterAnimInstance.h"
#include "MurdockAnimInstance.generated.h"

UCLASS()
class PROJECTJG_API UMurdockAnimInstance : public UCCharacterAnimInstance
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		bool IsFiring;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		float PistolHolster;

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
private:
	class AMurdock* Murdock;
};
