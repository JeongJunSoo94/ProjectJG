#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "PlayerWeaponHUDWidget.generated.h"

UCLASS()
class PROJECTJG_API UPlayerWeaponHUDWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized();

	UFUNCTION(BlueprintCallable)
		void SetInit(UTexture2D* image = nullptr);
	UFUNCTION(BlueprintCallable)
		void Update(float curValue, float maxValue);
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UImage* BackGroundImage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UTextBlock* TextBlock;
};
