// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SelectCharacter.generated.h"

UCLASS()
class PROJECTJG_API ASelectCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASelectCharacter();

protected:
	virtual void BeginPlay() override;

public:	


};
