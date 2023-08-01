#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidget_CrossHair.generated.h"

UCLASS()
class PROJECTJG_API UUserWidget_CrossHair : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
		void OnFocus();
	UFUNCTION(BlueprintImplementableEvent)
		void OffFocus();
};
