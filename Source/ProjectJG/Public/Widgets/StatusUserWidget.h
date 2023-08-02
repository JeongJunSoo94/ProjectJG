#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatusUserWidget.generated.h"

UCLASS()
class PROJECTJG_API UStatusUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
		void Update(float Health, float maxHealth);
};
