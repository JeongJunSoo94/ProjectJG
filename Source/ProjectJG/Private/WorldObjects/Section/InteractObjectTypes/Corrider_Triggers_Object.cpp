#include "WorldObjects/Section/InteractObjectTypes/Corrider_Triggers_Object.h"
#include "Global.h"
#include "Character/CBaseCharacter.h"
#include "Components/BoxComponent.h"
#include "WorldObjects/Section/InteractObjectTypes/Abstract_Spawn_Object.h"

void ACorrider_Triggers_Object::OnEndOverlap_StartBoxTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//Clog::Log("EndOverlap Section1 Trigger");
	if (OtherActor->IsA<ACBaseCharacter>())
	{
		SpawnEnemy();
		CallBackStartSection();
	}
}

void ACorrider_Triggers_Object::OnEndOverlap_EndBoxTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA<ACBaseCharacter>())
	{
		Clog::Log("Stop Spawner");
		StopSpawnEnemy();
		CallBackEndSection();
	}
}

ACorrider_Triggers_Object::ACorrider_Triggers_Object()
{
	CHelpers::CreateComponent<UBoxComponent>(this, &SectionStart_BoxTrigger, "SectionStart_BoxTrigger");
	//SectionStart_BoxTrigger->bHiddenInGame = false;
	CHelpers::CreateComponent<UBoxComponent>(this, &SectionEnd_BoxTrigger, "SectionEnd_BoxTrigger");
	//SectionEnd_BoxTrigger->bHiddenInGame = false;
}


void ACorrider_Triggers_Object::BeginPlay()
{
	SectionStart_BoxTrigger->OnComponentEndOverlap.AddDynamic(this, &ACorrider_Triggers_Object::OnEndOverlap_StartBoxTrigger);
	SectionEnd_BoxTrigger->OnComponentEndOverlap.AddDynamic(this, &ACorrider_Triggers_Object::OnEndOverlap_EndBoxTrigger);

}

void ACorrider_Triggers_Object::SettingSequences(ULevelSequence* Sequence, FMovieSceneSequencePlaybackSettings Settings, ETriggerType TriggerType)
{
	Super::SettingSequences(Sequence, Settings, TriggerType);
}

void ACorrider_Triggers_Object::PlayEndTrigger()
{
	// enemy all die;
}

void ACorrider_Triggers_Object::CallBackStartSection()
{
	OnStartSection.ExecuteIfBound();
}


void ACorrider_Triggers_Object::CallBackEndSection()
{
	OnEndSection.ExecuteIfBound();
	//StopSpawnEnemy();
}

void ACorrider_Triggers_Object::OnSequenceEvent_Implementation()
{

}
