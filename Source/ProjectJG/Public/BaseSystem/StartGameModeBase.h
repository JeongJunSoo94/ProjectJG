// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DataAssets/SelectCharacterDataAsset.h"
#include "StartGameModeBase.generated.h"

UCLASS()
class PROJECTJG_API AStartGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AStartGameModeBase();
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "SelectData")
		USelectCharacterDataAsset* SelectCharacterDataAsset;

	class AActor* Camera;
	class AActor* SpawnActor;

	int curIndexNum;
	TArray<AActor*> SelectCharacters;
public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	void SpawnSelectActor(int value);
protected:
	void SetActive(AActor* actor, bool value);
};
