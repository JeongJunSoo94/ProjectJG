// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "InGameModeBase.generated.h"

UENUM(BlueprintType)
enum class EGameState :uint8
{
	GameStart,
	GamePlay,
	GamePause,
	GameOver,
};

UCLASS()
class PROJECTJG_API AInGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
protected:
	AInGameModeBase();
	virtual void BeginPlay()override;

	UPROPERTY(EditAnywhere, Category = "GameModeBase")
		EGameState gameStateEnum;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
		TSubclassOf<UUserWidget>HUDWidgetClass;

	UUserWidget* CurWidget;

	UPROPERTY(VisibleDefaultsOnly)
		class UObjectPoolFactory* ObjectPoolFactory;
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString CharacterName;
};
