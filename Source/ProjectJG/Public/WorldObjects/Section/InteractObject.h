// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovieSceneSequencePlayer.h"
#include "InteractObject.generated.h"

UENUM(BlueprintType)
enum class ETriggerType : uint8
{
	None					= 0,
	Start					= 1,
	End						= 2,
	EveryTimeSectionEvent	= 3,
	DoingSectionEvent		= 4,
	BeforeSectionEvent		= 5,
	AfterSectionEvent		= 6
	
	

};

USTRUCT(BlueprintType)
struct FSequenceData
{
	GENERATED_BODY()

public:
	class ULevelSequencePlayer* LevelSequencePlayer = nullptr;
	class ALevelSequenceActor* LevelSequenceActor = nullptr;
	UPROPERTY(EditAnywhere)
		ETriggerType TriggerType = ETriggerType::None;
	UPROPERTY(EditAnywhere)
		FMovieSceneSequencePlaybackSettings MovieSettings;

	bool IsSet() { return LevelSequencePlayer && LevelSequenceActor; }
};

UCLASS()
class PROJECTJG_API AInteractObject : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere)
		TMap<class ULevelSequence* , FSequenceData> LevelSequenceArray;


public:	

	AInteractObject();

protected:
	virtual void BeginPlay() override;
	void PlaySequence(ETriggerType TriggerType = ETriggerType::None);
	virtual void SettingSequences(class ULevelSequence* Sequence, FMovieSceneSequencePlaybackSettings Settings, ETriggerType TriggerType = ETriggerType::None);

public:
	UFUNCTION(BlueprintCallable)
		void OnSequenceEvent();

	virtual void OnSequenceEvent_Implementation() {}
};
