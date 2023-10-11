// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseSystem/GameStateBase/InGameStateBase.h"
#include "Global.h"
#include "BaseSystem/PlayerState/InGamePlayerState.h"
#include "WorldObjects/Section/SectionMediator.h"

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
	if(SectionData!=curSection)
		curSection = SectionData;
}

void AInGameStateBase::EndSection()
{
	curSection->EndSectionEvent();
	// SectionEnd trigger Or End Effect ...
}
