// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldObjects/Section/InteractObject.h"
#include "WorldObjects/Section/SectionStart_Interface.h"
#include "WorldObjects/Section/SectionEnd_Interface.h"
#include "DoorObject.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTJG_API ADoorObject : public AInteractObject ,public ISectionStart_Interface, public ISectionEnd_Interface
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly)
		class UBoxComponent* BoxTrigger;

	UFUNCTION()
		void OnEndOverlapBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void BeginPlay() override;

	virtual void SettingSequences(class ULevelSequence* Sequence,class ALevelSequenceActor* OutActor) override;
public:
	ADoorObject();

	UFUNCTION()
		virtual void PlayEndTrigger() override;
};
