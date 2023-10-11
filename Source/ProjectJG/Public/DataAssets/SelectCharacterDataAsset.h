// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameFramework/Actor.h"
#include "SelectCharacterDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FSelectCharacterList
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> Enemy;
};

UCLASS()
class PROJECTJG_API USelectCharacterDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
};
