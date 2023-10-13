// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Character/QuestWidget.h"
#include "Global.h"

void UQuestWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GameState = Cast<AInGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	GameState->OnEventUpdateSection.BindUObject(this, &UQuestWidget::OnUpdateSection);
	GameState->OnEventUpdateData.BindUObject(this, &UQuestWidget::OnUpdateData);
}


void UQuestWidget::UpdateQuestData()
{
	if (!GameState)
	{
		GameState = Cast<AInGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	}

	if(!GameState->GetClearCondition())
	{
		Clog::Log("SectionMediation is Null");
		return;
	}
	ClearCondition = *(GameState->GetClearCondition());
}

FClearCondition UQuestWidget::GetClearCondition()
{
	return ClearCondition;
}