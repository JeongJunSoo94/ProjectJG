// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldObjects/Section/InteractObjectTypes/DoorObject.h"
#include "Global.h"
#include "Components/BoxComponent.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "BaseSystem/GameStateBase/InGameStateBase.h"

ADoorObject::ADoorObject()
{
	CHelpers::CreateComponent<UBoxComponent>(this, &SectionStart_BoxTrigger, "SectionStart_BoxTrigger");
	SectionStart_BoxTrigger->bHiddenInGame = false;
	CHelpers::CreateComponent<UBoxComponent>(this, &SectionEnd_BoxTrigger, "SectionEnd_BoxTrigger");
	SectionEnd_BoxTrigger->bHiddenInGame = false;

	IsStartInterface = true;
	IsEndInterface = true;
}

void ADoorObject::OnEndOverlapStartBoxTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Clog::Log("OnEndOverlapBox");
	PlaySequence(ETriggerType::Start);
	DoorOpenedActor = OtherActor;
}

void ADoorObject::OnBeginOverlapEndBoxTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AInGameStateBase* gameState = Cast<AInGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	gameState->EndSection();
}

void ADoorObject::BeginPlay()
{
	Super::BeginPlay();
	Clog::Log("BeginPlay Door Object");
	SectionStart_BoxTrigger->OnComponentEndOverlap.AddDynamic(this, &ADoorObject::OnEndOverlapStartBoxTrigger);

	SectionEnd_BoxTrigger->OnComponentBeginOverlap.AddDynamic(this, &ADoorObject::OnBeginOverlapEndBoxTrigger);
}

void ADoorObject::PlayEndTrigger()
{
	Clog::Log("PlayEndTrigger");

	PlaySequence(ETriggerType::End);
}


void ADoorObject::CallBackStartSection()
{
	OnTriggerSection.ExecuteIfBound();
}

void ADoorObject::SettingSequences(ULevelSequence* Sequence, FMovieSceneSequencePlaybackSettings Settings, ETriggerType TriggerType)
{
	ALevelSequenceActor* OutActor;
	//FMovieSceneSequencePlaybackSettings settings;
	//settings.bPauseAtEnd = true;

	ULevelSequencePlayer* Player = ULevelSequencePlayer::CreateLevelSequencePlayer(
		GetWorld(), Sequence, Settings, OutActor);

	Player->OnFinished.AddDynamic(this, &ADoorObject::CallBackStartSection);
	
	FSequenceData data;
	data.LevelSequencePlayer = Player;
	data.LevelSequenceActor = OutActor;
	data.TriggerType = TriggerType;

	
	LevelSequenceArray[Sequence] = data;

}

void ADoorObject::InteractObjectEvent_Implementation()
{
	Clog::Log("InteractObjectEvent_Implementation");
	CheckNull(DoorOpenedActor);
	SectionEnd_BoxTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}