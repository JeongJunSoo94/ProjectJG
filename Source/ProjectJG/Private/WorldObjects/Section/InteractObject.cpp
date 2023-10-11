
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

	for (TPair<ULevelSequence*, FSequenceData>& SequencePair : LevelSequenceArray)
	{
		Clog::Log("Setting Sequences");
		SettingSequences(SequencePair.Key, SequencePair.Value.MovieSettings, SequencePair.Value.TriggerType);
	}
	
}

void AInteractObject::PlaySequence(ETriggerType TriggerType)
{
	Clog::Log("Play All Sequence");
	//TPair<ULevelSequence*, ALevelSequenceActor*> SequencePair;
	
	for (TPair<ULevelSequence*, FSequenceData > &SequencePair : LevelSequenceArray)
	{
		if (!SequencePair.Value.IsSet())
		{
			SettingSequences(SequencePair.Key, SequencePair.Value.MovieSettings, SequencePair.Value.TriggerType);
		}

		if(TriggerType == SequencePair.Value.TriggerType)
			SequencePair.Value.LevelSequencePlayer->Play();
	}


}

void AInteractObject::SettingSequences(ULevelSequence* Sequence, FMovieSceneSequencePlaybackSettings Settings, ETriggerType TriggerType)
{

	ALevelSequenceActor* OutActor;
	//FMovieSceneSequencePlaybackSettings settings;
	//settings.bPauseAtEnd = true;

	ULevelSequencePlayer* Player = ULevelSequencePlayer::CreateLevelSequencePlayer(
		GetWorld(), Sequence, Settings, OutActor);

	FSequenceData data;
	data.LevelSequencePlayer = Player;
	data.LevelSequenceActor = OutActor;
	data.TriggerType = TriggerType;
	LevelSequenceArray[Sequence] = data;
}

void AInteractObject::OnSequenceEvent()
{
	OnSequenceEvent_Implementation();
}
