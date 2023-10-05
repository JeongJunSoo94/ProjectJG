// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldObjects/Section/InteractObjectTypes/DoorObject.h"
#include "Global.h"
#include "Components/BoxComponent.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlayer.h"
#include "LevelSequenceActor.h"


ADoorObject::ADoorObject()
{
	CHelpers::CreateComponent<UBoxComponent>(this, &BoxTrigger, "BoxTrigger");
	BoxTrigger->bHiddenInGame = false;
	IsStartInterface = true;
	IsEndInterface = true;
}

void ADoorObject::OnEndOverlapBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Clog::Log("OnEndOverlapBox");
	PlayAllSequence();
	OnTriggerSection.ExecuteIfBound();
}

void ADoorObject::BeginPlay()
{
	Clog::Log("BeginPlay Door Object");
	BoxTrigger->OnComponentEndOverlap.AddDynamic(this, &ADoorObject::OnEndOverlapBox);

	
}

void ADoorObject::PlayEndTrigger()
{
	Clog::Log("PlayEndTrigger");


}

void ADoorObject::SettingSequences(ULevelSequence* Sequence, ALevelSequenceActor* OutActor)
{
	FMovieSceneSequencePlaybackSettings settings;
	settings.bPauseAtEnd = true;

	

	ULevelSequencePlayer* Player = ULevelSequencePlayer::CreateLevelSequencePlayer(
		GetWorld(), Sequence, settings, OutActor);

	Player->OnFinished.AddDynamic(this, &ADoorObject::PlayEndTrigger);


	LevelSequenceArray[Sequence] = Player;

}
