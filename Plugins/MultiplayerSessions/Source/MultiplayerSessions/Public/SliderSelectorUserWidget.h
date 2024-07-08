// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SliderSelectorUserWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSelectDelegate, FString,Value);

UCLASS()
class MULTIPLAYERSESSIONS_API USliderSelectorUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
		class UButton* LeftArrowButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* RightArrowButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FName> Options;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SelectText;
	int CurIdx=0;

	UPROPERTY(BlueprintAssignable, Category = Delegates, meta = (AllowPrivateAccess = "true"))
		FSelectDelegate SelectDelegate;
protected:
	virtual void NativeConstruct() override;
	UFUNCTION()
	void OnNext();	
	UFUNCTION()
	void OnPrevious();
};
