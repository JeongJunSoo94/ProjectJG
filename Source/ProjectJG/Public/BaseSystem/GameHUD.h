#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Character/Components/CActionComponent.h"
#include "GameHUD.generated.h"

UENUM(BlueprintType)
enum class ECrosshairType : uint8
{
	CT_Normal UMETA(DisplayName = "Normal"),
	CT_Horizontal UMETA(DisplayName = "Horizontal"),
	CT_Vertical UMETA(DisplayName = "Vertical"),
	CT_VerticalHorizontal UMETA(DisplayName = "VerticalHorizontal"),
	CT_Diagonal UMETA(DisplayName = "Diagonal"),
	CT_Triangle UMETA(DisplayName = "Triangle"),
	CT_AllDirections UMETA(DisplayName = "AllDirections"),

	CT_MAX UMETA(DisplayName = "DefaultMAX")
};

USTRUCT(BlueprintType)
struct FHUDPackage
{
	GENERATED_BODY()
public:
	ECrosshairType CrosshairType;
	class UTexture2D* CrosshairsCenter;

	UTexture2D* CrosshairsLeft;
	UTexture2D* CrosshairsRight;
	UTexture2D* CrosshairsTop;
	UTexture2D* CrosshairsBottom;

	UTexture2D* CrosshairsTopLeft;
	UTexture2D* CrosshairsTopRight;
	UTexture2D* CrosshairsBottomLeft;
	UTexture2D* CrosshairsBottomRight;

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
	void AddPlayerInGameWidget();

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<class UAnnouncement> MatchAnnouncementWidgetClass;

	class UAnnouncement* MatchAnnouncement;
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

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<class UElimAnnouncement> ElimAnnouncementWidgetClass;

	UPROPERTY(EditAnywhere)
	float ElimAnnouncementTime = 2.5f;

	UFUNCTION()
	void ElimAnnouncementTimerFinished(UElimAnnouncement* MsgToRemove);

	UPROPERTY()
	TArray<UElimAnnouncement*> ElimMessages;


private:
	UPROPERTY()
		class APlayerController* OwningPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUDCrosshairPackage, meta = (AllowPrivateAccess = "true"))
		FHUDPackage HUDPackage;

	void DrawCrosshairNormal(const FVector2D& ViewportCenter,const float& SpreadScaled);
	void DrawCrosshairHorizontal(const FVector2D& ViewportCenter, const float& SpreadScaled);
	void DrawCrosshairVertical(const FVector2D& ViewportCenter, const float& SpreadScaled);
	void DrawCrosshairDiagonal(const FVector2D& ViewportCenter, const float& SpreadScaled);
	void DrawCrosshairTriangle(const FVector2D& ViewportCenter, const float& SpreadScaled);

	void DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor CrosshairColor);

	UPROPERTY(EditAnywhere)
		float CrosshairSpreadMax = 16.f;
public:
	virtual void SetHUDPlayerControllerSkillBind(UActorComponent* Weapon = nullptr, UCActionComponent* Left = nullptr, UCActionComponent* Mid = nullptr, UCActionComponent* Right = nullptr);
	virtual void HealthBarUpdate(float curHealth, float maxHealth);
	void IsMenuBarActive(bool isActive);
	void IsSoundActive(bool isActive);

	void AddAnnouncement();
	void AddElimAnnouncement(FString Attacker, FString Victim);
public:
	FORCEINLINE void SetHUDPackage(const FHUDPackage& Package) { HUDPackage = Package; }
	FORCEINLINE UPlayerInGameWidget* GetPlayerInGameWidget() { return PlayerInGameUI; }
	bool bPause = false;
};
