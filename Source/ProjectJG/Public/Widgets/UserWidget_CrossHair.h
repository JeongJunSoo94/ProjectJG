#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Engine/DataTable.h"
#include "GameFramework/Character.h"
#include "UserWidget_CrossHair.generated.h"

USTRUCT(BlueprintType)
struct FCrossHairDataTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UTexture2D* CrosshairsMiddle;
	UPROPERTY(meta = (BindWidget))
		UTexture2D* CrosshairsLeft;
	UPROPERTY(meta = (BindWidget))
		UTexture2D* CrosshairsRight;
	UPROPERTY(meta = (BindWidget))
		UTexture2D* CrosshairsBottom;
	UPROPERTY(meta = (BindWidget))
		UTexture2D* CrosshairsTop;
};

UCLASS()
class PROJECTJG_API UUserWidget_CrossHair : public UUserWidget
{
	GENERATED_BODY()
public:
	//UUserWidget_CrossHair();
	UFUNCTION(BlueprintImplementableEvent)
		void OnFocus();
	UFUNCTION(BlueprintImplementableEvent)
		void OffFocus();
	void CalculateCrosshairSpread(float DeltaTime, const FVector PawnVelocity, FVector2D WalkSpeedRange, FVector2D VelocityMultiplierRange);

	void StartCrosshairBulletFire();

	UFUNCTION()
		void FinishCrosshairBulletFire();
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
		UDataTable* WeaponDataTable;

	//>>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
		UTexture2D* CrosshairsMiddle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
		UTexture2D* CrosshairsLeft;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
		UTexture2D* CrosshairsRight;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
		UTexture2D* CrosshairsBottom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
		UTexture2D* CrosshairsTop;
	//<<

	//>>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = "true"))
		float CrosshairSpreadMultiplier;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = "true"))
		float CrosshairVelocityFactor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = "true"))
		float CrosshairInAirFactor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = "true"))
		float CrosshairAimFactor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = "true"))
		float CrosshairShootingFactor;
	//<<
	bool bAiming;
	bool bFiringBullet;
	ACharacter* character;

	float ShootTimeDuration;
	FTimerHandle CrosshairShootTimer;


};
