
#include "WorldObjects/Section/SectionMediator.h"
#include "Global.h"
#include "WorldObjects/Section/InteractObject.h"

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
		if (actor->IsStartInterface)
		{
			if (Cast<ISectionStart_Interface>(actor))
			{
				Clog::Log("InitSection : actor is startInterface, Not Null");
			}
			AddSectionStartTrigger(Cast<ISectionStart_Interface>(actor));
		}

		if (actor->IsEndInterface)
		{
			if (Cast<ISectionEnd_Interface>(actor))
			{
				Clog::Log("InitSection : actor is EndInterface, Not Null");
			}
			AddSectionEndTrigger(Cast<ISectionEnd_Interface>(actor));
		}
	}




	for (ISectionStart_Interface* StartTrigger : StartTriggers)
	{
		StartTrigger->OnTriggerSection.BindUObject(this, &ASectionMediator::StartSectionEvent);
		
	}
}
void ASectionMediator::StartSectionEvent()
{
	// ex) begin Spawn Enemy
	Clog::Log("Spawn enemy");
}

void ASectionMediator::OnSolveSectionProblem()
{
	// ex) kill Enemy
	Clog::Log("Solve Problem");
}


void ASectionMediator::EndSectionEvent()
{
	for (ISectionEnd_Interface* EndTrigger : EndTriggers)
	{
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