// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SectionStart_Interface.generated.h"

// This class does not need to be modified.

DECLARE_DELEGATE(FOnStartTrigger);

UINTERFACE(MinimalAPI)
class USectionStart_Interface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTJG_API ISectionStart_Interface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	FOnStartTrigger OnTriggerSection;
	UFUNCTION()
		virtual void CallBackStartSection() = 0;
};
