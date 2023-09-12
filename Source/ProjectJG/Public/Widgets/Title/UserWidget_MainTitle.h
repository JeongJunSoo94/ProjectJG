// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidget_MainTitle.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTJG_API UUserWidget_MainTitle : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class UVerticalBox* MainTitle_VerticalBox;

	virtual void NativeOnInitialized();

public:
	UFUNCTION(BlueprintCallable)
		void GetOutCurrentPanel();
	
	
};
