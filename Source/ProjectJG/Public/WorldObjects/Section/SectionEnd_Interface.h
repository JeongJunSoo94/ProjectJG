// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SectionEnd_Interface.generated.h"

DECLARE_DELEGATE(FOnStartTrigger);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USectionEnd_Interface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTJG_API ISectionEnd_Interface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	FOnStartTrigger OnEndSection;
	UFUNCTION()
		virtual void PlayEndTrigger() = 0;
};
