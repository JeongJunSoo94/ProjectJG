#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FloatingDamageWidget.generated.h"

UCLASS()
class PROJECTJG_API UFloatingDamageWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized();
	
	UFUNCTION(BlueprintImplementableEvent)
		void SetText(float damageValue);
};
