// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldObjects/Section/InteractObjectTypes/Corrider_Triggers_Object.h"
#include "Global.h"
#include "Character/CBaseCharacter.h"
#include "Components/BoxComponent.h"


void ACorrider_Triggers_Object::OnEndOverlapBoxTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//Clog::Log("EndOverlap Section1 Trigger");
	if (OtherActor->IsA<ACBaseCharacter>())
	{
		Clog::Log("OtherActor is a ACBaseCharacter");
		CallBackStartSection();
	}
}

ACorrider_Triggers_Object::ACorrider_Triggers_Object()
{
	CHelpers::CreateComponent<UBoxComponent>(this, &SectionStart_BoxTrigger, "SectionStart_BoxTrigger");
	SectionStart_BoxTrigger->bHiddenInGame = false;
}

void ACorrider_Triggers_Object::BeginPlay()
{
	SectionStart_BoxTrigger->OnComponentEndOverlap.AddDynamic(this, &ACorrider_Triggers_Object::OnEndOverlapBoxTrigger);

}

void ACorrider_Triggers_Object::SettingSequences(ULevelSequence* Sequence, FMovieSceneSequencePlaybackSettings Settings, ETriggerType TriggerType)
{
	Super::SettingSequences(Sequence, Settings, TriggerType);
}

void ACorrider_Triggers_Object::CallBackStartSection()
{
	OnTriggerSection.ExecuteIfBound();
}

void ACorrider_Triggers_Object::OnSequenceEvent_Implementation()
{

}
