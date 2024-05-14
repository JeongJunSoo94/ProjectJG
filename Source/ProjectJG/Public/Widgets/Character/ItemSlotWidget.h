// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Texture2D.h"
#include "ItemSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTJG_API UItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
public:
	UFUNCTION(BlueprintCallable)
		void UpdateItemSlotImage(UTexture2D* BackgroundImage, UTexture2D* ItemIcon, UTexture2D* AmmoIcon);
	UFUNCTION(BlueprintCallable)
		void SelectItemSlotImage(bool value);
	UFUNCTION(BlueprintCallable)
		void UpdateItemSlotCount(FString Count);
public:
	UPROPERTY(EditAnywhere)
		int32 SlotIndex;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* ItemCountText;
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
		class UImage* ArrowIcon;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* BackgroundImage;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* ItemIcon;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* AmmoIcon;
};
