// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseSystem/GameStateBase/InGameStateBase.h"
#include "Global.h"
#include "BaseSystem/PlayerState/InGamePlayerState.h"
#include "WorldObjects/Section/SectionMediator.h"
#include "Widgets/Character/QuestWidget.h"

void AInGameStateBase::ChangeGameState(EGameState NextState)
{
	switch (curState)
	{
	case EGameState::GameStart:
	{
		if (NextState == EGameState::GamePlay)
		{
			curState = NextState;
		}

	}
		break;
	case EGameState::GamePlay:
	{
		if (NextState == EGameState::GamePause|| NextState == EGameState::GameOver)
		{
			curState = NextState;
		}
	}
		break;
	case EGameState::GamePause:
	{
		if (NextState == EGameState::GamePlay || NextState == EGameState::GameOver)
		{
			curState = NextState;
		}
	}
		break;
	case EGameState::GameOver:
	{
		if (NextState == EGameState::GameStart)
		{
			curState = NextState;
		}
	}
		break;
	default:
		break;
	}
}

void AInGameStateBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	for (APlayerState* PlayerState : PlayerArray)
	{
		AInGamePlayerState* InGamePlayerState = Cast<AInGamePlayerState>(PlayerState);
		if (!!InGamePlayerState)
			InGamePlayerArray.AddUnique(InGamePlayerState);
	}

	//curSectionData = DefaultSectionData
	ChangeGameState(EGameState::GamePlay);
}

void AInGameStateBase::StartSection(ASectionMediator *SectionData)
{
	if (SectionData != curSection)
	{
		curSection = SectionData;
		Map_SectionClearConditions.Add(SectionData, SectionData->ClearCondition);
		OnEventUpdateSection.ExecuteIfBound();
	}

}

bool AInGameStateBase::EndSection()
{
	if (!curSection)
	{
		Clog::Log("Section not set");
		return false;
	}
	if (Map_SectionClearConditions[curSection].IsGoal())
	{
		curSection->EndSectionEvent();
		return true;
	}
	return false;
	// SectionEnd trigger Or End Effect ...
}

void AInGameStateBase::TakeKey(uint8 KeyNumber)
{
	Map_SectionClearConditions[curSection].StoreKey(KeyNumber);
	OnEventUpdateData.ExecuteIfBound();
}

FClearCondition* AInGameStateBase::GetClearCondition()
{
	if (!curSection )
		return nullptr;
	return &Map_SectionClearConditions[curSection];
}

void AInGameStateBase::UpdateKillScore()
{
	CheckNull(curSection);
	Map_SectionClearConditions[curSection].KillEnemy(1);
	OnEventUpdateData.ExecuteIfBound();
}

void FClearCondition::StoreKey(uint8 KeyNumber)
{
	curKeyValue |= KeyNumber;
}

void FClearCondition::KillEnemy(int killScore)
{
	curKillNumber += killScore;
}

bool FClearCondition::IsGoal()
{
	return ((GoalKillNumber <= curKillNumber || GoalKillNumber == -1) && (GoalKeyValue == curKeyValue || GoalKeyValue == 0));
}
