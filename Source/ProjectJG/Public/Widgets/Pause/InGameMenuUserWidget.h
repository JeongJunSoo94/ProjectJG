#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameMenuUserWidget.generated.h"

UCLASS()
class PROJECTJG_API UInGameMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UInGameMenuUserWidget(const FObjectInitializer& ObjectInitializer);
	UFUNCTION(BlueprintCallable)
		void Resume();
	UFUNCTION(BlueprintCallable)
		void Lobby();
	UFUNCTION(BlueprintCallable)
		void Option();
	UFUNCTION(BlueprintCallable)
		void Exit();
};
