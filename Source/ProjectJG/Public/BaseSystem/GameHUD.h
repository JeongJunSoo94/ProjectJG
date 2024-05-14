#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Character/Components/CActionComponent.h"
#include "GameHUD.generated.h"

USTRUCT(BlueprintType)
struct FHUDPackage
{
	GENERATED_BODY()
public:
	class UTexture2D* CrosshairsCenter;
	UTexture2D* CrosshairsLeft;
	UTexture2D* CrosshairsRight;
	UTexture2D* CrosshairsTop;
	UTexture2D* CrosshairsBottom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CrosshairSpread;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor CrosshairsColor;
};

UCLASS()
class PROJECTJG_API AGameHUD : public AHUD
{
	GENERATED_BODY()
public:
	AGameHUD();

	virtual void DrawHUD() override;

	virtual void BeginPlay()override;
private:
	TSubclassOf<class UUserWidget>BP_UIWidgetClass;
	UUserWidget* UIWidget;

	//UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	//	TSubclassOf<class UUserWidget_CrossHair> CrossHairClass;

	//class UUserWidget_CrossHair* CrossHair;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<class UPlayerInGameWidget> PlayerInGameClass;

	class UPlayerInGameWidget* PlayerInGameUI;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<class UInGameMenuUserWidget> InGameMenuClass;

	class UInGameMenuUserWidget* InGameMenuUI;


	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<class USoundControlWidget> SoundControlClass;

	class USoundControlWidget* SoundControlUI;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<class UQuestWidget> QuestWidgetClass;

	class UQuestWidget* QuestWidgetUI;


private:
	UPROPERTY()
		class APlayerController* OwningPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUDCrosshairPackage, meta = (AllowPrivateAccess = "true"))
		FHUDPackage HUDPackage;

	void DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor CrosshairColor);

	UPROPERTY(EditAnywhere)
		float CrosshairSpreadMax = 16.f;
public:
	virtual void SetHUDPlayerControllerSkillBind(UActorComponent* Weapon = nullptr, UCActionComponent* Left = nullptr, UCActionComponent* Mid = nullptr, UCActionComponent* Right = nullptr);
	virtual void HealthBarUpdate(float curHealth, float maxHealth);
	void IsMenuBarActive(bool isActive);
	void IsSoundActive(bool isActive);
public:
	FORCEINLINE void SetHUDPackage(const FHUDPackage& Package) { HUDPackage = Package; }
	FORCEINLINE UPlayerInGameWidget* GetPlayerInGameWidget() { return PlayerInGameUI; }
	bool bPause = false;
};
