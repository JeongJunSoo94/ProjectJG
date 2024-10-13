// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldObjects/Item/Item.h"
#include "WorldObjects/Item/WeaponType.h"
#include "WorldObjects/Item/AmmoType.h"
#include "Engine/DataTable.h"
#include "Weapon.generated.h"

//구조 변경해야함 item 상태가 메인이고 아래 enum은 보조 수단
UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial UMETA(DisplayName = "Initial State"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_EquippedSecondary UMETA(DisplayName = "Equipped Secondary"),
	EWS_Dropped UMETA(DisplayName = "Dropped"),

	EWS_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EFireType : uint8
{
	EFT_HitScan UMETA(DisplayName = "Hit Scan Weapon"),
	EFT_Projectile UMETA(DisplayName = "Projectile Weapon"),
	EFT_Shotgun UMETA(DisplayName = "Shotgun Weapon"),

	EFT_MAX UMETA(DisplayName = "DefaultMAX")
};

USTRUCT(BlueprintType)
struct FWeaponDataTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 WeaponAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 MagazingCapacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 HeadDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundCue* PickupSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundCue* EquipSound;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	class UWidgetComponent* PickupWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMesh* ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* InventoryIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* AmmoIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterialInstance* MaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 MaterialIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ClipBoneName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ReloadMontageSection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UAnimInstance> AnimBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCurveFloat* SlideDisplacementCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxSlideDisplacement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimationAsset* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* CrosshairsCenter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* CrosshairsLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* CrosshairsRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* CrosshairsBottom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* CrosshairsTop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float FireDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName BoneToHide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ACase> CaseClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UParticleSystem* CaseParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundCue* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName MuzzleSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName AmmoEjectSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName MainHandSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName SubHandSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName CharacterAttachRightHandSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ZoomedFOV;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ZoomInterpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EWeaponType WeaponType;
	
};


UCLASS()
class PROJECTJG_API AWeapon : public AItem
{
	GENERATED_BODY()
public:
	AWeapon();
	virtual void Tick(float DeltaTime) override;

	//shooting
	FVector GetBeamTraceDirection(const FVector& HitTarget);
	bool GetBeamTraceDirection(const FVector& OutStart, FVector& OutHitLocation);
	//>>
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnRep_Owner() override;
	virtual void Fire(const FVector& HitTarget);
	void SetHUDAmmo();
	//void ShowPickupWidget(bool bShowWidget);
	//virtual void Fire(const FVector& HitTarget);
	virtual void Dropped() override;
	void AddAmmo(int32 AmmoToAdd);
	//<<
protected:
	void StopFalling();

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;

	void FinishMovingSlide();
	void UpdateSlideDisplacement();
private:
	FTimerHandle ThrowWeaponTimer;
	float ThrowWeaponTime;
	bool bFalling;

	//UPROPERTY(EditAnywhere)
	//	TSubclassOf<class AProjectile> ProjectileClass;

	//UPROPERTY(EditAnywhere)
	//	TSubclassOf<AProjectile> ServerSideRewindProjectileClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACase> CaseClass;

	UFUNCTION(Client, Reliable)
		void ClientUpdateAmmo(int32 ServerAmmo);

	UFUNCTION(Client, Reliable)
		void ClientAddAmmo(int32 Amount);

	int32 Sequence = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
		int32 Ammo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
		int32 MagazineCapacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
		EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
		FName ReloadMontageSection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
		bool bMovingClip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
		FName ClipBoneName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
		FName MainHandSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
		FName SubHandSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
		FName CharacterAttachRightHandSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
		UDataTable* WeaponDataTable;


	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
		class UAnimationAsset* FireAnimation;

	int32 PreviousMaterialIndex;
	//>>

	//<<
	//>>
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	//	float AutoFireRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
		FName MuzzleSocketName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
		FName AmmoEjectSocketName;
	//<<

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
		FName BoneToHide;

		/** Amount that the slide is pushed back during pistol fire */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Slide, meta = (AllowPrivateAccess = "true"))
		float SlideDisplacement;

		/** Curve for the slide displacement */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Slide, meta = (AllowPrivateAccess = "true"))
		UCurveFloat* SlideDisplacementCurve;

		/** Timer handle for updating SlideDisplacement */
		FTimerHandle SlideTimer;

		/** Time for displacing the slide during pistol fire */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Slide, meta = (AllowPrivateAccess = "true"))
		float SlideDisplacementTime;

		/** True when moving the pistol slide */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Slide, meta = (AllowPrivateAccess = "true"))
		bool bMovingSlide;

		/** Max distance for the slide on the pistol */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Slide, meta = (AllowPrivateAccess = "true"))
		float MaxSlideDisplacement;

		/** Max rotation for pistol recoil */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Slide, meta = (AllowPrivateAccess = "true"))
		float MaxRecoilRotation;

		/** Amount that the pistol will rotate during pistol fire*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Slide, meta = (AllowPrivateAccess = "true"))
		float RecoilRotation;

protected:
	//>>
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	//class UParticleSystem* ImpactParticles;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	//class UParticleSystem* BeamParticles;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* CaseParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* MuzzleFlash;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	USoundCue* FireSound;
	//<<

	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
		float DistanceToSphere = 800.f;

	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
		float SphereRadius = 75.f;

	UPROPERTY(EditAnywhere)
		float Damage = 20.f;

	UPROPERTY(EditAnywhere)
		float HeadShotDamage = 40.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
		FName WeaponName;

	UPROPERTY(EditAnywhere, Category = Combat)
		float FireDelay = .15f;
	//UPROPERTY(ReplicatedUsing = OnRep_WeaponState, VisibleAnywhere, Category = "Weapon Properties")
	//	EWeaponState WeaponState;
	//UFUNCTION()
	//	void OnRep_WeaponState();
	virtual void SetItemProperties(EItemState State) override;
	//virtual void OnEquipped();
	//virtual void OnWeaponStateSet();
public:
	//>>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
		UTexture2D* CrosshairsCenter;
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

	UPROPERTY(EditAnywhere, Category = Combat)
		bool bAutomatic = true;

	UPROPERTY(EditAnywhere)
		EFireType FireType;

	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
		bool bUseScatter = false;

	UPROPERTY(EditAnywhere)
		float ZoomedFOV = 30.f;

	UPROPERTY(EditAnywhere)
		float ZoomInterpSpeed = 20.f;

	//<<

public:
	//void SetWeaponState(EWeaponState State);
	void ThrowWeapon();
	FORCEINLINE float GetDamage() const { return Damage; }
	FORCEINLINE float GetHeadShotDamage() const{ return HeadShotDamage; }
	FORCEINLINE int32 GetAmmo() const { return Ammo; }
	FORCEINLINE int32 GetMagazineCapacity() const { return MagazineCapacity; }

	void SendBullet();

	void DecrementAmmo();

	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }
	FORCEINLINE FName GetReloadMontageSection() const { return ReloadMontageSection; }
	FORCEINLINE void SetReloadMontageSection(FName Name) { ReloadMontageSection = Name; }
	FORCEINLINE FName GetClipBoneName() const { return ClipBoneName; }
	FORCEINLINE void SetClipBoneName(FName Name) { ClipBoneName = Name; }
	FORCEINLINE float GetFireDelay() const { return FireDelay; }
	//new
	//FORCEINLINE UParticleSystem* GetMuzzleFlash() const { return MuzzleFlash; }
	//FORCEINLINE USoundCue* GetFireSound() const { return FireSound; }
	bool IsEmpty();
	void ReloadAmmo(int32 Amount);

	void StartSlideTimer();

	FORCEINLINE float GetZoomedFOV() const { return ZoomedFOV; }
	FORCEINLINE float GetZoomInterpSpeed() const { return ZoomInterpSpeed; }

	FORCEINLINE void SetMovingClip(bool Move) { bMovingClip = Move; }
	FORCEINLINE FName GetMuzzleSocketName() const { return MuzzleSocketName; }
	FORCEINLINE FName GetAmmoEjectSocketName() const { return AmmoEjectSocketName; }
	FORCEINLINE FName GetMainHandSocketName() const { return MainHandSocketName; }
	FORCEINLINE FName GetSubHandSocketName() const { return SubHandSocketName; }
	FORCEINLINE FName GetCharacterAttachRightHandSocketName() const { return CharacterAttachRightHandSocketName; }
	bool ClipIsFull();
};
