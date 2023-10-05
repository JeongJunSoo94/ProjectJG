
#include "WorldObjects/Section/InteractObject.h"
#include "Global.h"

#include "MovieSceneSequencePlayer.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"


AInteractObject::AInteractObject()
{

}

void AInteractObject::BeginPlay()
{
	Super::BeginPlay();

	ALevelSequenceActor* OutActor = nullptr; //temp parameter for function 

	for (TPair<ULevelSequence*, ULevelSequencePlayer*>& SequencePair : LevelSequenceArray)
	{
		Clog::Log("Setting Sequences");
		SettingSequences(SequencePair.Key, OutActor);
	}
	
}

void AInteractObject::PlayAllSequence()
{
	Clog::Log("Play All Sequence");
	ALevelSequenceActor* OutActor = nullptr;  //temp parameter for function 

	//TPair<ULevelSequence*, ALevelSequenceActor*> SequencePair;
	
	for (TPair<ULevelSequence*, ULevelSequencePlayer*> &SequencePair : LevelSequenceArray)
	{
		Clog::Log(SequencePair.Value);
		if (SequencePair.Value)
		{
			SequencePair.Value->Play();
		}
		else
		{
			SettingSequences(SequencePair.Key, OutActor);
		}
	}


}

void AInteractObject::SettingSequences(ULevelSequence* Sequence, ALevelSequenceActor* OutActor)
{
	FMovieSceneSequencePlaybackSettings settings;
	settings.bPauseAtEnd = true;

	ULevelSequencePlayer* Player = ULevelSequencePlayer::CreateLevelSequencePlayer(
		GetWorld(), Sequence, settings, OutActor);

	LevelSequenceArray[Sequence] = Player;
}
