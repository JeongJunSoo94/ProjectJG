// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldObjects/Section/InteractObject.h"
#include "WorldObjects/Section/SectionStart_Interface.h"
#include "WorldObjects/Section/SectionEnd_Interface.h"
#include "Corrider_Triggers_Object.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTJG_API ACorrider_Triggers_Object : public AInteractObject, public ISectionStart_Interface
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleDefaultsOnly)
		class UBoxComponent* SectionStart_BoxTrigger;
	
	UFUNCTION()
		void OnEndOverlapBoxTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	ACorrider_Triggers_Object();

	virtual void BeginPlay() override;

	virtual void SettingSequences(class ULevelSequence* Sequence, FMovieSceneSequencePlaybackSettings Settings, ETriggerType TriggerType) override;

	UFUNCTION()
		virtual void CallBackStartSection() override;

	virtual void OnSequenceEvent_Implementation() override;
};
