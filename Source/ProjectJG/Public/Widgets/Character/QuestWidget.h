// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseSystem/GameStateBase/InGameStateBase.h"
#include "QuestWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTJG_API UQuestWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized();

	UFUNCTION(BlueprintCallable)
		void UpdateQuestData();

	UFUNCTION(BlueprintCallable)
		FClearCondition GetClearCondition();

	UFUNCTION(BlueprintImplementableEvent)
		void OnUpdateSection();

	UFUNCTION(BlueprintImplementableEvent)
		void OnUpdateData();

	FClearCondition ClearCondition;
		
	AInGameStateBase* GameState;
};
