// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/Character/ItemSlotWidget.h"
#include "InventoryBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTJG_API UInventoryBarWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	//UFUNCTION(BlueprintCallable)
	//	void StartIconAnim();

	UFUNCTION(BlueprintCallable)
		UItemSlotWidget* GetSlotWidget(int32 Index);
public:
	UPROPERTY(BlueprintReadWrite, Category = "Slot", meta = (BindWidget))
		class UItemSlotWidget* WeaponSlot1;
	UPROPERTY(BlueprintReadWrite, Category = "Slot", meta = (BindWidget))
		class UItemSlotWidget* WeaponSlot2;
	UPROPERTY(BlueprintReadWrite, Category = "Slot", meta = (BindWidget))
		class UItemSlotWidget* WeaponSlot3;
	UPROPERTY(BlueprintReadWrite, Category = "Slot", meta = (BindWidget))
		class UItemSlotWidget* WeaponSlot4;
	UPROPERTY(BlueprintReadWrite, Category = "Slot", meta = (BindWidget))
		class UItemSlotWidget* WeaponSlot5;
	UPROPERTY(BlueprintReadWrite, Transient, Category = "SlotAnimation", meta = (BindWidgetAnim))
		class UWidgetAnimation* Weapon1Animation;
	UPROPERTY(BlueprintReadWrite, Transient, Category = "SlotAnimation", meta = (BindWidgetAnim))
		class UWidgetAnimation* Weapon2Animation;
	UPROPERTY(BlueprintReadWrite, Transient, Category = "SlotAnimation", meta = (BindWidgetAnim))
		class UWidgetAnimation* Weapon3Animation;
	UPROPERTY(BlueprintReadWrite, Transient, Category = "SlotAnimation", meta = (BindWidgetAnim))
		class UWidgetAnimation* Weapon4Animation;
	UPROPERTY(BlueprintReadWrite, Transient, Category = "SlotAnimation", meta = (BindWidgetAnim))
		class UWidgetAnimation* Weapon5Animation;

};
