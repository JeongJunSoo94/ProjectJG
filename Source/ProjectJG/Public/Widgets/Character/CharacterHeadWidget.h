#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterHeadWidget.generated.h"

UCLASS()
class PROJECTJG_API UCharacterHeadWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* DisplayText;

	void SetDisplayText(FString TextToDisplay);

	void SetPlayerText();

	UFUNCTION(BlueprintCallable)
		void ShowPlayerNetRole(APawn* InPawn);

protected:
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;
private:
	APawn* Owner;
	FString LocalRoleStr;
	FString RemoteRoleStr;
	FTimerHandle NameMatchTimer;
};
