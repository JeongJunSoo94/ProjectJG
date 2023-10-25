// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldObjects/Section/InteractObjectTypes/Abstract_Spawn_Object.h"
#include "WorldObjects/Section/SectionStart_Interface.h"
#include "WorldObjects/Section/SectionEnd_Interface.h"
#include "MovieSceneSequencePlayer.h"
#include "DoorObject.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTJG_API ADoorObject : public AAbstract_Spawn_Object,public ISectionStart_Interface, public ISectionEnd_Interface
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly)
		class UBoxComponent* SectionStart_BoxTrigger;
	UPROPERTY(VisibleDefaultsOnly)
		class UBoxComponent* SectionEnd_BoxTrigger;
	UPROPERTY(EditAnywhere)
		AActor* DoorOpenedActor;

	UFUNCTION()
		void OnEndOverlapStartBoxTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
		void OnBeginOverlapEndBoxTrigger( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	class AInGameStateBase* gameState;

	virtual void BeginPlay() override;

	virtual void SettingSequences(class ULevelSequence* Sequence, FMovieSceneSequencePlaybackSettings Settings, ETriggerType TriggerType) override;
public:
	ADoorObject();

	UFUNCTION()
		virtual void PlayEndTrigger() override;

	UFUNCTION()
		void CallBackStartSection();

	virtual void OnSequenceEvent_Implementation() override;
};
