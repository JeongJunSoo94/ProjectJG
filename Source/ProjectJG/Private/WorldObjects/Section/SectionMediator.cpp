
#include "WorldObjects/Section/SectionMediator.h"
#include "Global.h"
#include "WorldObjects/Section/InteractObject.h"
#include "BaseSystem/GameStateBase/InGameStateBase.h"

ASectionMediator::ASectionMediator()
{

}

void ASectionMediator::BeginPlay()
{
	Super::BeginPlay();

	InitSection();
}

void ASectionMediator::InitSection()
{
	for (AInteractObject* actor : TriggerActors)
	{
		if (!actor)
		{
			continue;
		}

		ISectionStart_Interface* StartInterface = Cast<ISectionStart_Interface>(actor);
		if (!!StartInterface)
		{
			Clog::Log("InitSection : actor is startInterface, Not Null");
			AddSectionStartTrigger(StartInterface);
		}
		
		ISectionEnd_Interface* EndInterface = Cast<ISectionEnd_Interface>(actor);
		if (!!EndInterface)
		{
			Clog::Log("InitSection : actor is startInterface, Not Null");
			AddSectionEndTrigger(EndInterface);
		}
	}

	for (ISectionStart_Interface* StartTrigger : StartTriggers)
	{
		StartTrigger->OnStartSection.BindUObject(this, &ASectionMediator::StartSectionEvent);
	}

	for (ISectionEnd_Interface* EndTrigger : EndTriggers)
	{
		EndTrigger->OnEndSection.BindUObject(this, &ASectionMediator::EndSectionEvent);
	}

}
void ASectionMediator::StartSectionEvent()
{
	for (ISectionStart_Interface* StartTrigger : StartTriggers)
	{
		StartTrigger->OnStartSection.Unbind();
	}

	Clog::Log("StartSectionEvent");
	AInGameStateBase* gameState = Cast<AInGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	gameState->StartSection(this);
}

void ASectionMediator::OnSolveSectionProblem()
{
	// ex) kill Enemy

	Clog::Log("Solve Problem");
	EndSectionEvent();
}


void ASectionMediator::EndSectionEvent()
{
	for (ISectionEnd_Interface* EndTrigger : EndTriggers)
	{
		EndTrigger->OnEndSection.Unbind();
		EndTrigger->PlayEndTrigger();
	}
}

void ASectionMediator::AddSectionStartTrigger(ISectionStart_Interface* StartTrigger)
{
	StartTriggers.AddUnique(StartTrigger);
}

void ASectionMediator::AddSectionEndTrigger(ISectionEnd_Interface* EndTrigger)
{
	EndTriggers.AddUnique(EndTrigger);
}