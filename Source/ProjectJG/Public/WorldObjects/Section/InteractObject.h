// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "InteractObject.generated.h"

UCLASS()
class PROJECTJG_API AInteractObject : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere)
		TMap<class ULevelSequence* , class ULevelSequencePlayer*> LevelSequenceArray;
public:	

	AInteractObject();

	bool IsStartInterface = false;
	bool IsEndInterface = false;
protected:
	virtual void BeginPlay() override;
	void PlayAllSequence();
	virtual void SettingSequences(class ULevelSequence* Sequence, class ALevelSequenceActor* OutActor);
};
