// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillComponent.generated.h"

UENUM(BlueprintType)
enum class ESkillState : uint8
{
	None = 0,
	Fire = 1,
	Aim = 2,
	QAbliity = 3,
	EAbliity = 4,
	RAbliity = 5,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTJG_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USkillComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly)
	class UCActionComponent* QAbility;
	UPROPERTY(VisibleDefaultsOnly)
	class UEAbliltyActionComponent* LtBelicaEAbility;
	UPROPERTY(VisibleDefaultsOnly)
	class URAbilityActionComponent* LtBelicaRAbility;
	UPROPERTY(EditAnywhere)
	ESkillState eSkillState;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
