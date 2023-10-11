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
class PROJECTJG_API ACorrider_Triggers_Object : public AAbstract_Spawn_Object, public ISectionStart_Interface, public ISectionEnd_Interface
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleDefaultsOnly)
		class UBoxComponent* SectionStart_BoxTrigger;
	UPROPERTY(VisibleDefaultsOnly)
		class UBoxComponent* SectionEnd_BoxTrigger;

	UFUNCTION()
		void OnEndOverlap_StartBoxTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void OnEndOverlap_EndBoxTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);



public:
	ACorrider_Triggers_Object();

	virtual void BeginPlay() override;

	virtual void SettingSequences(class ULevelSequence* Sequence, FMovieSceneSequencePlaybackSettings Settings, ETriggerType TriggerType) override;

	UFUNCTION()
		virtual void PlayEndTrigger() override;

	UFUNCTION()
		virtual void CallBackStartSection();
	UFUNCTION()
		virtual void CallBackEndSection();

	virtual void OnSequenceEvent_Implementation() override;
};
