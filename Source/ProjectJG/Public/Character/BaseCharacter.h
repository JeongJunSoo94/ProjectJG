// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Character.h"
#include "WorldObjects/Item/AmmoType.h"
#include "Widgets/Character/CharacterHeadWidget.h"
//#include "Character/Animation/CharacterAnimInstance.h"
#include "WorldObjects/Item/Weapon.h"
#include "Character/CombatState.h"
#include "Character/TurningInPlace.h"
#include "BaseCharacter.generated.h"

UENUM(BlueprintType)
enum class EMoveState : uint8
{
	EMS_NonCombat UMETA(DisplayName = "NonCombat"),
	EMS_Combat UMETA(DisplayName = "Combat"),

	EMS_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class ECharacterWeaponEquipState : uint8
{
	ECWES_Exclusive UMETA(DisplayName = "Exclusive"),
	ECWES_Multiple UMETA(DisplayName = "Multiple"),

	ECWES_MAX UMETA(DisplayName = "DefaultMAX")
};

USTRUCT(BlueprintType)
struct FCharacterTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ECharacterWeaponEquipState ECWES;
};

USTRUCT(BlueprintType)
struct FInterpLocation
{
	GENERATED_BODY()

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 ItemCount;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEquipItemDelegate, int32, CurrentSlotIndex, int32, NewSlotIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHighlightIconDelegate, int32, SlotIndex, bool, bStartAnimation);

UCLASS()
class PROJECTJG_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	UPROPERTY()
		TMap<FName, class UBoxComponent*> HitCollisionBoxes;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Axis);

	void MoveRight(float Axis);

	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

	void Turn(float Value);

	void LookUp(float Value);

	//>>
	void FireWeapon();

	void AimingButtonPressed();
	void AimingButtonReleased();

	void AimOffset(float DeltaTime);
	void CalculateAO_Pitch();
	void SimProxiesTurn();
	void Aim();
	void StopAiming();

	void CameraInterpZoom(float DeltaTime);

	void SetLookRates();

	void CalculateCrosshairSpread(float DeltaTime);

	void StartCrosshairBulletFire();

	UFUNCTION()
		void FinishCrosshairBulletFire();

	void FireButtonPressed();
	void FireButtonReleased();

	//void StartFireTimer();

	//UFUNCTION()
	//	void AutoFireReset();

	//<<

	void TraceForItems();

	//>>
	//item weapon
	//class AWeapon* SpawnDefaultWeapon();
	
	void EquipWeapon(AWeapon* WeaponToEquip, bool bSwapping = false);

	void DropWeapon();

	void SelectButtonPressed();
	void SelectButtonReleased();
	void SpawDefaultWeapon();
	void SwapWeapon(AWeapon* WeaponToSwap);
	//<<
	
	//>>
	//item ammo
	void InitializeAmmoMap();
	//<<

	//>>
	UFUNCTION(BlueprintCallable)
		void FinishReloading();

	UFUNCTION(BlueprintCallable)
		void FinishEquipping();
	//<<

	void CrouchButtonPressed();
	void InterpCapsuleHalfHeight(float DeltaTime);

	void InitailizeInterpLocations();

	virtual void Jump() override;

	int32 GetEmptyInventorySlot();

	void HighlightInventorySlot();

	void SetSpawnPoint();
	void OnPlayerStateInitialized();

	UFUNCTION()
		void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);

	void PollInit();

	void RotateInPlace(float DeltaTime);

	void OneKeyPressed();

	void TwoKeyPressed();

	void ThreeKeyPressed();

	void FourKeyPressed();

	void FiveKeyPressed();

	void PlayHitReactMontage();

	void QKeyPressed();
	void EKeyPressed();
	void RKeyPressed();

	UPROPERTY(EditAnywhere)
		class UBoxComponent* head;

	UPROPERTY(EditAnywhere)
		UBoxComponent* pelvis;

	UPROPERTY(EditAnywhere)
		UBoxComponent* spine_02;

	UPROPERTY(EditAnywhere)
		UBoxComponent* spine_03;

	UPROPERTY(EditAnywhere)
		UBoxComponent* upperarm_l;

	UPROPERTY(EditAnywhere)
		UBoxComponent* upperarm_r;

	UPROPERTY(EditAnywhere)
		UBoxComponent* lowerarm_l;

	UPROPERTY(EditAnywhere)
		UBoxComponent* lowerarm_r;

	UPROPERTY(EditAnywhere)
		UBoxComponent* hand_l;

	UPROPERTY(EditAnywhere)
		UBoxComponent* hand_r;

	UPROPERTY(EditAnywhere)
		UBoxComponent* thigh_l;

	UPROPERTY(EditAnywhere)
		UBoxComponent* thigh_r;

	UPROPERTY(EditAnywhere)
		UBoxComponent* calf_l;

	UPROPERTY(EditAnywhere)
		UBoxComponent* calf_r;

	UPROPERTY(EditAnywhere)
		UBoxComponent* foot_l;

	UPROPERTY(EditAnywhere)
		UBoxComponent* foot_r;

	//>>
	float AO_Yaw;
	float InterpAO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;

	ETurningInPlace TurningInPlace;
	void TurnInPlace(float DeltaTime);

	//<<

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;

	//>>montage
	void PlayFireMontage(bool bAiming);
	void PlayReloadMontage();
	void PlayElimMontage();
	void PlaySwapMontage();
	//void PlayThrowGrenadeMontage();
	//<<

	virtual void OnRep_ReplicatedMovement() override;

	bool TraceScreenCrosshairCollision(FHitResult& OutHitResult, FVector& OutHitLocation);

	void UpdateHUDHealth();
	void UpdateHUDShield();
	void UpdateHUDAmmo();
protected:
	//UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraSpringArm;
	//UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* CharacterCamera;

private:
	//>>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float BaseLookUpRate;
	//<<

	//>>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float HipTurnRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float HipLookUpRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float AimingTurnRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float AimingLookUpRate;
	//<<

	//>>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
		float MouseHipTurnRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
		float MouseHipLookUpRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
		float MouseAimingTurnRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
		float MouseAimingLookUpRate;
	//<<

	//>>
	//State
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	//	bool bAiming;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Move, meta = (AllowPrivateAccess = "true"))
		bool bMove;
	//<<
	
	////>>
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = "true"))
	//	float CrosshairSpreadMultiplier;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = "true"))
	//	float CrosshairVelocityFactor;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = "true"))
	//	float CrosshairInAirFactor;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = "true"))
	//	float CrosshairAimFactor;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = "true"))
	//	float CrosshairShootingFactor;
	////<<

	//>>

	//>>
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	//	float CameraDefaultFOV;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	//	float CameraZoomedFOV;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	//	float CameraCurrentFOV;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	//	float ZoomInterpSpeed;
	//<<

	//>>
	//Fire
	float ShootTimeDuration;
	bool bFiringBullet;
	FTimerHandle CrosshairShootTimer;

	bool bShouldFire;

	bool bFireButtonPressed;

	bool bAimingButtonPressed;


	FTimerHandle AutoFireTimer;

	//<<
	//>>
	//Item
	UPROPERTY(ReplicatedUsing = OnRep_TraceItem)
		class AItem* TraceHitItem;
	UFUNCTION()
		void OnRep_TraceItem(AItem* LastTraceHitItem);

	UFUNCTION(Server, Reliable)
		void ServerSelectButtonPressed();

	UFUNCTION(Server, Reliable)
		void ServerNumberButtonPressed(int32 slotIndex);

	bool bShouldTraceForItems;

	int8 OverlappedItemCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
		class AItem* TraceHitItemLastFrame;

	//<<

	//>>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
		TMap<EAmmoType, int32>AmmoMap;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
		int32 Starting9mmAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
		int32 StartingARAmmo;
	//<<

	//>>

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCombatComponent* Combat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UBuffComponent* Buff;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class ULagCompensationComponent* LagCompensation;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	//	AWeapon* EquippedWeapon;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	//	TSubclassOf<AWeapon> DefaultWeaponClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
		ECombatState CombatState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* HipFireMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* ReloadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* EquipMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* ElimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* SwapMontage;

	bool bRotateRootBone;
	float TurnThreshold = 0.5f;
	FRotator ProxyRotationLastFrame;
	FRotator ProxyRotation;
	float ProxyYaw;
	float TimeSinceLastMovementReplication;
	float CalculateSpeed();

	//<<

	//>>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		bool bCrouching;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float BaseMovementSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float CrouchMovementSpeed;
	//<<

	//>>
	float CurrentCapsuleHalfHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float StandingCapsuleHalfHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float CrouchingCapsuleHalfHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float BaseGroundFriction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float CrouchingGroundFriction;
	//<<

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterHeadInfo, meta = (AllowPrivateAccess = "true"))
		class UWidgetComponent* CharacterHeadWidget;

	class UCharacterHeadWidget* CacheCharacterHeadWidget;

	class UCharacterAnimInstance* CacheCharacterAnimInstance;

	FTimerHandle PickupSoundTimer;
	FTimerHandle EquipSoundTimer;

	bool bShouldPlayPickupSound;
	bool bShouldPlayEquipSound;

	void ResetPickupSoundTimer();
	void ResetEquipSoundTimer();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
		float PickupSoundResetTime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
		float EquipSoundResetTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TArray<FInterpLocation> InterpLocations;

	UPROPERTY(BlueprintAssignable, Category = Delegates, meta = (AllowPrivateAccess = "true"))
		FEquipItemDelegate EquipItemDelegate;

	UPROPERTY(BlueprintAssignable, Category = Delegates, meta = (AllowPrivateAccess = "true"))
		FHighlightIconDelegate HighlightIconDelegate;

	UPROPERTY(ReplicatedUsing = OnRep_Inventory,VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
		TArray<AItem*> Inventory;
	UFUNCTION()
		void OnRep_Inventory();

	const int32 INVENTORY_CAPACITY{ 5 };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
		int32 HighlightedSlot;
	
	UPROPERTY(EditAnywhere, Category = "Player Stats")
		float MaxHealth = 100.f;

	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "Player Stats")
		float Health = 100.f;

	UFUNCTION()
		void OnRep_Health(float LastHealth);

	UPROPERTY(EditAnywhere, Category = "Player Stats")
		float MaxShield = 100.f;

	UPROPERTY(ReplicatedUsing = OnRep_Shield, EditAnywhere, Category = "Player Stats")
		float Shield = 0.f;

	UFUNCTION()
		void OnRep_Shield(float LastShield);

	UPROPERTY()
		class AInGamePlayerController* BasePlayerController;

	UPROPERTY()
		class ABattleGameMode* BattleGameMode;

	UPROPERTY()
		class AInGamePlayerState* BasePlayerState;

	bool bElimmed = false;

	FTimerHandle ElimTimer;

	UPROPERTY(EditDefaultsOnly)
		float ElimDelay = 3.f;
public:
	FORCEINLINE USpringArmComponent* GetCameraSpringArm() const { return CameraSpringArm; }
	FORCEINLINE UCameraComponent* GetCharacterCamera() const { return CharacterCamera; }
	FORCEINLINE UCharacterHeadWidget* GetCharacterHeadWidget() const { return CacheCharacterHeadWidget; }
	FORCEINLINE UCombatComponent* GetCombatComp() const { return Combat; }
	FORCEINLINE ULagCompensationComponent* GetLagCompensation() const { return LagCompensation; }
	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE void SetHealth(float Amount) { Health = Amount; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	FORCEINLINE float GetShield() const { return Shield; }
	FORCEINLINE void SetShield(float Amount) { Shield = Amount; }
	FORCEINLINE float GetMaxShield() const { return MaxShield; }

	FORCEINLINE bool IsElimmed() const { return bElimmed; }
	//FORCEINLINE bool GetAiming() const { return bAiming; }
	//UFUNCTION(BlueprintCallable)
	//	float GetCrosshairSpreadMultiplier() const;

	FORCEINLINE ECombatState GetCombatState() const { return CombatState; }
	FORCEINLINE bool GetCrouching() const { return bCrouching; }
	FInterpLocation GetInterpLocation(int32 Index);

	int32 GetInterpLocationIndex();

	FORCEINLINE bool ShouldPlayPickupSound() const { return bShouldPlayPickupSound; }
	FORCEINLINE bool ShouldPlayEquipSound() const { return bShouldPlayEquipSound; }

	void GetPickupItem(AItem* Item);

	void StartPickupSoundTimer();
	void StartEquipSoundTimer();

	void IncrementInterpLocItemCount(int32 Index, int32 Amount);

	void UnHighlightInventorySlot();

	void IncrementOverlappedItemCount(int8 Amount);

	bool bFinishedSwapping = false;

	bool IsAiming();

	void SetFindCamera();

	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }
	FORCEINLINE bool GetRotateRootBone() const { return bRotateRootBone; }
	FORCEINLINE ETurningInPlace GetTurningInPlace() const{ return TurningInPlace; }
	bool IsLocallyReloading();

	void InitializeHitBoxsMap();
};
