// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "InGameModeBase.generated.h"


UCLASS()
class PROJECTJG_API AInGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
protected:
	AInGameModeBase();
	virtual void BeginPlay()override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
		TSubclassOf<UUserWidget>HUDWidgetClass;

	UUserWidget* CurWidget;

	UPROPERTY(VisibleDefaultsOnly)
		class UObjectPoolFactory* ObjectPoolFactory;
public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString CharacterName;
	UPROPERTY(EditAnywhere)
		class UCharacterDataAsset* characterDataAsset;

	TArray<TQueue<AActor*>> Test;
};
