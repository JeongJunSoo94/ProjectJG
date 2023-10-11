// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldObjects/Section/SectionStart_Interface.h"
#include "WorldObjects/Section/SectionEnd_Interface.h"
#include "SectionMediator.generated.h"


UENUM(BlueprintType,meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ESectionState : uint8
{
	ENone = 0,
	EProceedingSection = 1 << 0,
	EClearSection = 1 << 1
};

UCLASS()
class PROJECTJG_API ASectionMediator : public AActor
{
	GENERATED_BODY()
	
public:	

	ASectionMediator();
	UPROPERTY(EditAnywhere, Category = "TriggerActors")
		TArray <class AInteractObject*> TriggerActors;

	UPROPERTY(EditAnywhere, Category = "SpawnerActors")
		TArray<class ASpawnerActor*> SpawnerActors;

protected:
	virtual void BeginPlay() override;

		TArray<ISectionStart_Interface*> StartTriggers;
		TArray<ISectionEnd_Interface*> EndTriggers;

public:
	virtual void InitSection();

	UFUNCTION(BlueprintCallable)
		virtual void StartSectionEvent();
	UFUNCTION(BlueprintCallable)
		virtual void OnSolveSectionProblem();

	virtual void EndSectionEvent();

	virtual void AddSectionStartTrigger(ISectionStart_Interface* StartTrigger);
	virtual void AddSectionEndTrigger(ISectionEnd_Interface* EndTrigger);
};
