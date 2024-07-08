#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/CharacterSkillWidget.h"
#include "Widgets/Character/InventoryBarWidget.h"
#include "Widgets/Character/ItemInfoWidget.h"
#include "Character/Components/CActionComponent.h"
#include "PlayerInGameWidget.generated.h"

UCLASS()
class PROJECTJG_API UPlayerInGameWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintCallable)
		void HealthUpdate(float CurHealth, float MaxHealth);

	UFUNCTION(BlueprintCallable)
		void ShieldUpdate(float CurShield, float MaxShield);

	UFUNCTION(BlueprintCallable)
		void SetItemInfoCount(int32 ItemCount);

	UFUNCTION(BlueprintCallable)
		void SetItemInfoCarried(int32 Carried);

	UFUNCTION(BlueprintCallable)
		void SetItemInfoType(FString ItemType);
public:
	UPROPERTY(meta = (BindWidget))
		UTextBlock* ScoreAmount;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* RedTeamScore;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* BlueTeamScore;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* ScoreSpacerText;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* DefeatsAmount;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* MatchCountdownText;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* GrenadesText;

protected:
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* HealthText;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* ShieldBar;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ShieldText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UCharacterSkillWidget* WB_CharacterSkill_Left;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UCharacterSkillWidget* WB_CharacterSkill_Mid;

	//UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	//	class UInventoryBarWidget* InventoryBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UItemInfoWidget* ItemInfo;

private:
	class ABaseCharacter* player;

public:
	FORCEINLINE ABaseCharacter* GetCharacter() { return player; }
	FORCEINLINE UCharacterSkillWidget* GetWB_CharacterSkill_Left() { return WB_CharacterSkill_Left; }
	FORCEINLINE UCharacterSkillWidget* GetWB_CharacterSkill_Mid() { return WB_CharacterSkill_Mid; }
	//FORCEINLINE UCharacterSkillWidget* GetWB_CharacterSkill_Right() { return WB_CharacterSkill_Right; }
	//FORCEINLINE UInventoryBarWidget* GetInventoryBar() { return InventoryBar; }
	//FORCEINLINE UItemInfoWidget* GetItemInfo() { return ItemInfo; }
};
