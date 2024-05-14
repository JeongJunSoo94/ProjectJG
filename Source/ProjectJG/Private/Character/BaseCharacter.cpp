#include "Character/BaseCharacter.h"
#include "Global.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "WorldObjects/Item/Item.h"
#include "WorldObjects/Item/Weapon.h"
#include "WorldObjects/Item/Ammo.h"
#include "Components/WidgetComponent.h"
#include "Widgets/Character/CharacterHeadWidget.h"
#include "Character/Animation/CharacterAnimInstance.h"
#include "Character/Components/CombatComponent.h"
#include "Net/UnrealNetwork.h"
#include "BaseSystem/InGamePlayerController.h"
#include "BaseSystem/BattleGameMode.h"
#include "BaseSystem/PlayerState/InGamePlayerState.h"
#include "Character/Components/BuffComponent.h"
#include "Character/Components/LagCompensationComponent.h"
#include "Character/Animation/CharacterAnimInstance.h"

ABaseCharacter::ABaseCharacter() :
	//Base rates
	BaseTurnRate(45.f),
	BaseLookUpRate(45.f),
	//Turn rates
	HipTurnRate(90.f),
	HipLookUpRate(90.f),
	AimingTurnRate(20.f),
	AimingLookUpRate(20.f),
	TurningInPlace(ETurningInPlace::ETIP_NotTurning),
	// Mouse look sensitivity scale factors 
	MouseHipTurnRate(1.0f),
	MouseHipLookUpRate(1.0f),
	MouseAimingTurnRate(0.6f),
	MouseAimingLookUpRate(0.6f),
	//Combat control
	//bAiming(false),
	//Move control
	bMove(true),
	//Crosshair spread factors
	//CrosshairSpreadMultiplier(0.f),
	//CrosshairVelocityFactor(0.f),
	//CrosshairInAirFactor(0.f),
	//CrosshairAimFactor(0.f),
	//CrosshairShootingFactor(0.f),
	//Camera field of view values
	/*CameraDefaultFOV(0.f),
	CameraZoomedFOV(25.f),
	CameraCurrentFOV(0.f),
	ZoomInterpSpeed(20.f),*/
	//Bullet fire timer
	ShootTimeDuration(0.05f),
	bFiringBullet(false),
	//Automatic fire variables
	bShouldFire(true),
	bFireButtonPressed(false),
	//combat
	CombatState(ECombatState::ECS_Unoccupied),
	//Crouch
	bCrouching(false),
	//
	BaseMovementSpeed(650.f),
	CrouchMovementSpeed(300.f),
	//
	StandingCapsuleHalfHeight(80.f),
	CrouchingCapsuleHalfHeight(44.f),
	BaseGroundFriction(2.f),
	CrouchingGroundFriction(100.f)
{
	PrimaryActorTick.bCanEverTick = true;

	//>>
	//camera
	CHelpers::CreateComponent<USpringArmComponent>(this, &CameraSpringArm, "CameraSpringArm", RootComponent);
	CameraSpringArm->SetupAttachment(RootComponent);
	CameraSpringArm->TargetArmLength = 180.f;
	CameraSpringArm->bUsePawnControlRotation = true;
	CameraSpringArm->SocketOffset = FVector(0.f, 50.f, 70.f);

	CHelpers::CreateComponent<UCameraComponent>(this, &CharacterCamera, "CharacterCamera", CameraSpringArm);
	CharacterCamera->SetupAttachment(CameraSpringArm, USpringArmComponent::SocketName);
	CharacterCamera->bUsePawnControlRotation = false;
	//<<

	//bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	//bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	//NetInfo Widget
	CharacterHeadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("CharacterHeadWidget"));
	CharacterHeadWidget->SetupAttachment(RootComponent);

	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat"));
	Combat->SetIsReplicated(true);

	Buff = CreateDefaultSubobject<UBuffComponent>(TEXT("BuffComponent"));
	Buff->SetIsReplicated(true);

	LagCompensation = CreateDefaultSubobject<ULagCompensationComponent>(TEXT("LagCompensation"));

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetCharacterMovement()->RotationRate = FRotator(0.f, 0.f, 850.f);

	TurningInPlace = ETurningInPlace::ETIP_NotTurning;
	NetUpdateFrequency = 66.f;
	MinNetUpdateFrequency = 33.f;

	head = CreateDefaultSubobject<UBoxComponent>(TEXT("head"));
	head->SetupAttachment(GetMesh(), FName("head"));
	HitCollisionBoxes.Add(FName("head"), head);

	pelvis = CreateDefaultSubobject<UBoxComponent>(TEXT("pelvis"));
	pelvis->SetupAttachment(GetMesh(), FName("pelvis"));
	HitCollisionBoxes.Add(FName("pelvis"), pelvis);

	spine_02 = CreateDefaultSubobject<UBoxComponent>(TEXT("spine_02"));
	spine_02->SetupAttachment(GetMesh(), FName("spine_02"));
	HitCollisionBoxes.Add(FName("spine_02"), spine_02);

	spine_03 = CreateDefaultSubobject<UBoxComponent>(TEXT("spine_03"));
	spine_03->SetupAttachment(GetMesh(), FName("spine_03"));
	HitCollisionBoxes.Add(FName("spine_03"), spine_03);

	upperarm_l = CreateDefaultSubobject<UBoxComponent>(TEXT("upperarm_l"));
	upperarm_l->SetupAttachment(GetMesh(), FName("upperarm_l"));
	HitCollisionBoxes.Add(FName("upperarm_l"), upperarm_l);

	upperarm_r = CreateDefaultSubobject<UBoxComponent>(TEXT("upperarm_r"));
	upperarm_r->SetupAttachment(GetMesh(), FName("upperarm_r"));
	HitCollisionBoxes.Add(FName("upperarm_r"), upperarm_r);

	lowerarm_l = CreateDefaultSubobject<UBoxComponent>(TEXT("lowerarm_l"));
	lowerarm_l->SetupAttachment(GetMesh(), FName("lowerarm_l"));
	HitCollisionBoxes.Add(FName("lowerarm_l"), lowerarm_l);

	lowerarm_r = CreateDefaultSubobject<UBoxComponent>(TEXT("lowerarm_r"));
	lowerarm_r->SetupAttachment(GetMesh(), FName("lowerarm_r"));
	HitCollisionBoxes.Add(FName("lowerarm_r"), lowerarm_r);

	hand_l = CreateDefaultSubobject<UBoxComponent>(TEXT("hand_l"));
	hand_l->SetupAttachment(GetMesh(), FName("hand_l"));
	HitCollisionBoxes.Add(FName("hand_l"), hand_l);

	hand_r = CreateDefaultSubobject<UBoxComponent>(TEXT("hand_r"));
	hand_r->SetupAttachment(GetMesh(), FName("hand_r"));
	HitCollisionBoxes.Add(FName("hand_r"), hand_r);

	thigh_l = CreateDefaultSubobject<UBoxComponent>(TEXT("thigh_l"));
	thigh_l->SetupAttachment(GetMesh(), FName("thigh_l"));
	HitCollisionBoxes.Add(FName("thigh_l"), thigh_l);

	thigh_r = CreateDefaultSubobject<UBoxComponent>(TEXT("thigh_r"));
	thigh_r->SetupAttachment(GetMesh(), FName("thigh_r"));
	HitCollisionBoxes.Add(FName("thigh_r"), thigh_r);

	calf_l = CreateDefaultSubobject<UBoxComponent>(TEXT("calf_l"));
	calf_l->SetupAttachment(GetMesh(), FName("calf_l"));
	HitCollisionBoxes.Add(FName("calf_l"), calf_l);

	calf_r = CreateDefaultSubobject<UBoxComponent>(TEXT("calf_r"));
	calf_r->SetupAttachment(GetMesh(), FName("calf_r"));
	HitCollisionBoxes.Add(FName("calf_r"), calf_r);

	foot_l = CreateDefaultSubobject<UBoxComponent>(TEXT("foot_l"));
	foot_l->SetupAttachment(GetMesh(), FName("foot_l"));
	HitCollisionBoxes.Add(FName("foot_l"), foot_l);

	foot_r = CreateDefaultSubobject<UBoxComponent>(TEXT("foot_r"));
	foot_r->SetupAttachment(GetMesh(), FName("foot_r"));
	HitCollisionBoxes.Add(FName("foot_r"), foot_r);

	for (auto Box : HitCollisionBoxes)
	{
		if (Box.Value)
		{
			Box.Value->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);
			Box.Value->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			Box.Value->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Block);
			Box.Value->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	CHelpers::CheckNullComponent<UCameraComponent>(this, &CharacterCamera);
	//if (CharacterCamera)
	//{
	//	CameraDefaultFOV = GetCharacterCamera()->FieldOfView;
	//}
	//else
	//{
	//	CHelpers::CheckNullComponent<UCameraComponent>(this, &CharacterCamera);
	//	CameraDefaultFOV = GetCharacterCamera()->FieldOfView;
	//}

	CharacterHeadWidget->InitWidget();
	CacheCharacterHeadWidget = Cast<UCharacterHeadWidget>(CharacterHeadWidget->GetUserWidgetObject());
	CacheCharacterAnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance());

	Combat->SetCharacter(this);
	//Combat->SpawnDefaultWeapon();
	//Inventory.Add(Combat->GetEquippedWeapon());
	if (Combat->GetEquippedWeapon())
	{
		Combat->GetEquippedWeapon()->SetSlotIndex(0);
		Combat->GetEquippedWeapon()->EnableCustomDepth(false);
		Combat->GetEquippedWeapon()->DisableGlowMaterial();
		Combat->GetEquippedWeapon()->SetCharacter(this);
	}
	InitializeAmmoMap();

	GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;

	if (HasAuthority())
	{
		OnTakeAnyDamage.AddDynamic(this, &ABaseCharacter::ReceiveDamage);
	}
	//InitailizeInterpLocations();
}

void ABaseCharacter::InitializeHitBoxsMap()
{
	TArray<UActorComponent*> Boxs= GetComponentsByClass(UBoxComponent::StaticClass());
	for (auto& box : Boxs)
	{
		if (box->GetName().Equals("head"))
			HitCollisionBoxes.Add(FName("head"), head);
		else if (box->GetName().Equals("pelvis"))
			HitCollisionBoxes.Add(FName("pelvis"), pelvis);
		else if (box->GetName().Equals("spine_02"))
			HitCollisionBoxes.Add(FName("spine_02"), spine_02);
		else if (box->GetName().Equals("spine_03"))
			HitCollisionBoxes.Add(FName("spine_03"), spine_03);
		else if (box->GetName().Equals("upperarm_l"))
			HitCollisionBoxes.Add(FName("upperarm_l"), upperarm_l);
		else if (box->GetName().Equals("upperarm_r"))
			HitCollisionBoxes.Add(FName("upperarm_r"), upperarm_r);
		else if (box->GetName().Equals("lowerarm_l"))
			HitCollisionBoxes.Add(FName("lowerarm_l"), lowerarm_l);
		else if (box->GetName().Equals("lowerarm_r"))
			HitCollisionBoxes.Add(FName("lowerarm_r"), lowerarm_r);
		else if (box->GetName().Equals("hand_l"))
			HitCollisionBoxes.Add(FName("hand_l"), hand_l);
		else if (box->GetName().Equals("hand_r"))
			HitCollisionBoxes.Add(FName("hand_r"), hand_r);
		else if (box->GetName().Equals("thigh_l"))
			HitCollisionBoxes.Add(FName("thigh_l"), thigh_l);
		else if (box->GetName().Equals("thigh_r"))
			HitCollisionBoxes.Add(FName("thigh_r"), thigh_r);
		else if (box->GetName().Equals("calf_l"))
			HitCollisionBoxes.Add(FName("calf_l"), calf_l);
		else if (box->GetName().Equals("calf_r"))
			HitCollisionBoxes.Add(FName("calf_r"), calf_r);
		else if (box->GetName().Equals("foot_l"))
			HitCollisionBoxes.Add(FName("foot_l"), foot_l);
		else if (box->GetName().Equals("foot_r"))
			HitCollisionBoxes.Add(FName("foot_r"), foot_r);
	}
	for (auto Box : HitCollisionBoxes)
	{
		if (Box.Value)
		{
			Box.Value->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);
			Box.Value->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			Box.Value->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Block);
			Box.Value->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

void ABaseCharacter::MoveForward(float Axis)
{
	//CheckFalse(bMove);
	if ((Controller != nullptr) && (Axis != 0.0f))
	{
		const FRotator YawRotation{ 0, Controller->GetControlRotation().Yaw, 0 };
		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::X) };
		AddMovementInput(Direction, Axis);
	}
}

void ABaseCharacter::MoveRight(float Axis) 
{
	if ((Controller != nullptr) && (Axis != 0.0f))
	{
		const FRotator YawRotation{ 0, Controller->GetControlRotation().Yaw,0 };
		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::Y) };
		AddMovementInput(Direction, Axis);
	}
}

void ABaseCharacter::TurnAtRate(float Rate) 
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ABaseCharacter::LookUpAtRate(float Rate) 
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ABaseCharacter::Turn(float Value) 
{
	float TurnScaleFactor{};
	if (Combat->GetAiming())
	{
		TurnScaleFactor = MouseAimingTurnRate;
	}
	else
	{
		TurnScaleFactor = MouseHipTurnRate;
	}
	AddControllerYawInput(Value * TurnScaleFactor);
}

void ABaseCharacter::LookUp(float Value) 
{
	float LookUpScaleFactor{};
	if (Combat->GetAiming())
	{
		LookUpScaleFactor = MouseAimingLookUpRate;
	}
	else
	{
		LookUpScaleFactor = MouseHipLookUpRate;
	}
	AddControllerPitchInput(Value * LookUpScaleFactor);
}

void ABaseCharacter::FireWeapon()
{
	//if (Combat->GetEquippedWeapon() == nullptr) return;
	//if (CombatState != ECombatState::ECS_Unoccupied) return;
	//if (Combat->GetEquippedWeapon()->GetItemCount()>0)
	//{
	//	Combat->FireButtonPressed(true);
	//	//PlayFireSound();
	//	//EquippedWeapon->SendBullet();
	//	PlayFireMontage(true);
	//	//EquippedWeapon->DecrementAmmo();
	//	//StartFireTimer();
	//}
}

void ABaseCharacter::AimingButtonPressed()
{
	//bAimingButtonPressed = true;
	//if (Combat && Combat->bHoldingTheFlag) return;
	//if (bDisableGameplay) return;
	if (Combat)
	{
		Combat->SetAiming(true);
	}
}

void ABaseCharacter::AimingButtonReleased()
{
	//bAimingButtonPressed = false;
	//StopAiming();
	//if (Combat && Combat->bHoldingTheFlag) return;
	//if (bDisableGameplay) return;
	if (Combat)
	{
		Combat->SetAiming(false);
	}
}

float ABaseCharacter::CalculateSpeed()
{
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f;
	return Velocity.Size();
}

void ABaseCharacter::AimOffset(float DeltaTime)
{
	if (Combat && Combat->GetEquippedWeapon() == nullptr) return;
	float Speed = CalculateSpeed();
	bool bIsInAir = GetCharacterMovement()->IsFalling();

	if (Speed == 0.f && !bIsInAir) // standing still, not jumping
	{
		bRotateRootBone = true;
		FRotator CurrentAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation, StartingAimRotation);
		AO_Yaw = DeltaAimRotation.Yaw;
		if (TurningInPlace == ETurningInPlace::ETIP_NotTurning)
		{
			InterpAO_Yaw = AO_Yaw;
		}
		bUseControllerRotationYaw = true;
		TurnInPlace(DeltaTime);
	}
	if (Speed > 0.f || bIsInAir) // running, or jumping
	{
		bRotateRootBone = false;
		StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		AO_Yaw = 0.f;
		bUseControllerRotationYaw = true;
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
	}

	CalculateAO_Pitch();
}

void ABaseCharacter::CalculateAO_Pitch()
{
	AO_Pitch = GetBaseAimRotation().Pitch;
	if (AO_Pitch > 90.f && !IsLocallyControlled())
	{
		// map pitch from [270, 360) to [-90, 0)
		FVector2D InRange(270.f, 360.f);
		FVector2D OutRange(-90.f, 0.f);
		AO_Pitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AO_Pitch);
	}
}

void ABaseCharacter::SimProxiesTurn()
{
	if (Combat == nullptr || Combat->EquippedWeapon == nullptr) return;
	bRotateRootBone = false;
	float Speed = CalculateSpeed();
	if (Speed > 0.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
		return;
	}

	ProxyRotationLastFrame = ProxyRotation;
	ProxyRotation = GetActorRotation();
	ProxyYaw = UKismetMathLibrary::NormalizedDeltaRotator(ProxyRotation, ProxyRotationLastFrame).Yaw;

	if (FMath::Abs(ProxyYaw) > TurnThreshold)
	{
		if (ProxyYaw > TurnThreshold)
		{
			TurningInPlace = ETurningInPlace::ETIP_Right;
		}
		else if (ProxyYaw < -TurnThreshold)
		{
			TurningInPlace = ETurningInPlace::ETIP_Left;
		}
		else
		{
			TurningInPlace = ETurningInPlace::ETIP_NotTurning;
		}
		return;
	}
	TurningInPlace = ETurningInPlace::ETIP_NotTurning;

}

void ABaseCharacter::TurnInPlace(float DeltaTime)
{
	if (AO_Yaw > 90.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_Right;
	}
	else if (AO_Yaw < -90.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_Left;
	}
	if (TurningInPlace != ETurningInPlace::ETIP_NotTurning)
	{
		InterpAO_Yaw = FMath::FInterpTo(InterpAO_Yaw, 0.f, DeltaTime, 4.f);
		AO_Yaw = InterpAO_Yaw;
		if (FMath::Abs(AO_Yaw) < 15.f)
		{
			TurningInPlace = ETurningInPlace::ETIP_NotTurning;
			StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		}
	}
}

void ABaseCharacter::Aim()
{
	//bAiming = true;
	//GetCharacterMovement()->MaxWalkSpeed = CrouchMovementSpeed;
}

void ABaseCharacter::StopAiming()
{
	//bAiming = false;
	//if (!bCrouching)
	//{
	//	GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;
	//}
}

void ABaseCharacter::CameraInterpZoom(float DeltaTime)
{
	//if (bAiming)
	//{
	//	CameraCurrentFOV = FMath::FInterpTo(CameraCurrentFOV,CameraZoomedFOV,DeltaTime,ZoomInterpSpeed);
	//}
	//else
	//{
	//	CameraCurrentFOV = FMath::FInterpTo(CameraCurrentFOV,CameraDefaultFOV,DeltaTime,ZoomInterpSpeed);
	//}
	//GetCharacterCamera()->SetFieldOfView(CameraCurrentFOV);
}

void ABaseCharacter::SetLookRates()
{
	if (Combat->GetAiming())
	{
		BaseTurnRate = AimingTurnRate;
		BaseLookUpRate = AimingLookUpRate;
	}
	else
	{
		BaseTurnRate = HipTurnRate;
		BaseLookUpRate = HipLookUpRate;
	}
}

void ABaseCharacter::CalculateCrosshairSpread(float DeltaTime)
{
	//FVector2D WalkSpeedRange{ 0.f,600.f };
	//FVector2D VelocityMultiplierRange{ 0.f,1.f };
	//FVector Velocity{ GetVelocity() };
	//Velocity.Z = 0.f;

	//CrosshairVelocityFactor = FMath::GetMappedRangeValueClamped(WalkSpeedRange, VelocityMultiplierRange, Velocity.Size());

	//if (GetCharacterMovement()->IsFalling())
	//{
	//	CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 2.25f, DeltaTime, 2.25f);
	//}
	//else
	//{
	//	CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 0.f, DeltaTime, 30.f);
	//}

	//if (bAiming)
	//{
	//	CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor, 0.6f, DeltaTime, 30.f);
	//}
	//else
	//{
	//	CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor, 0.f, DeltaTime, 30.f);
	//}

	//if (bFiringBullet)
	//{
	//	CrosshairShootingFactor = FMath::FInterpTo(CrosshairShootingFactor, 0.3f, DeltaTime, 60.f);
	//}
	//else
	//{
	//	CrosshairShootingFactor = FMath::FInterpTo(CrosshairShootingFactor, 0.f, DeltaTime, 60.f);
	//}

	//CrosshairSpreadMultiplier = 0.5f + CrosshairVelocityFactor + CrosshairInAirFactor - CrosshairAimFactor + CrosshairShootingFactor;
}

void ABaseCharacter::StartCrosshairBulletFire()
{
	bFiringBullet = true;
	GetWorldTimerManager().SetTimer(CrosshairShootTimer, this, &ABaseCharacter::FinishCrosshairBulletFire, ShootTimeDuration);
}

void ABaseCharacter::FinishCrosshairBulletFire()
{
	bFiringBullet = false;
}

void ABaseCharacter::FireButtonPressed()
{
	bFireButtonPressed = true;
	if (Combat)
	{
		Combat->FireButtonPressed(true);
	}
}

void ABaseCharacter::FireButtonReleased()
{
	bFireButtonPressed = false;
	if (Combat)
	{
		Combat->FireButtonPressed(false);
	}
}

void ABaseCharacter::TraceForItems()
{
	if (bShouldTraceForItems)
	{
		FHitResult ItemTraceResult;
		FVector HitLocation;
		TraceScreenCrosshairCollision(ItemTraceResult, HitLocation);
		if (ItemTraceResult.bBlockingHit)
		{
			TraceHitItem = Cast<AItem>(ItemTraceResult.GetActor());
			const auto TraceHitWeapon = Cast<AWeapon>(TraceHitItem);

			if (TraceHitWeapon)
			{
				if (HighlightedSlot == -1)
				{
					HighlightInventorySlot();
				}
			}
			else
			{
				if (HighlightedSlot != -1)
				{
					UnHighlightInventorySlot();
				}
			}
			if (TraceHitItem && TraceHitItem->GetItemState() == EItemState::EIS_EquipInterping)
			{
				TraceHitItem = nullptr;
			}

			if (TraceHitItem && TraceHitItem->GetPickupWidget())
			{
				if (IsLocallyControlled())
				{
					TraceHitItem->GetPickupWidget()->SetVisibility(true);
					TraceHitItem->EnableCustomDepth(true);
					if (Inventory.Num() >= INVENTORY_CAPACITY)
					{
						TraceHitItem->SetCharacterInventoryFull(true);
					}
					else
					{
						TraceHitItem->SetCharacterInventoryFull(false);
					}
				}
			}

			if (TraceHitItemLastFrame)
			{
				if (TraceHitItem != TraceHitItemLastFrame)
				{
					TraceHitItemLastFrame->GetPickupWidget()->SetVisibility(false);
					TraceHitItemLastFrame->EnableCustomDepth(false);
				}
			}

			TraceHitItemLastFrame = TraceHitItem;
		}
	}
	else if (TraceHitItemLastFrame)
	{
		TraceHitItemLastFrame->GetPickupWidget()->SetVisibility(false);
		TraceHitItemLastFrame->EnableCustomDepth(false);
	}
}

void ABaseCharacter::EquipWeapon(AWeapon* WeaponToEquip, bool bSwapping)
{
	//if (WeaponToEquip)
	//{
	//	//WeaponToEquip->GetAreaSphere()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	//	//WeaponToEquip->GetCollisionBox()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);


	//	const USkeletalMeshSocket* HandSocket = GetMesh()->GetSocketByName(FName("RightHandSocket"));
	//	if (HandSocket)
	//	{
	//		HandSocket->AttachActor(WeaponToEquip, GetMesh());
	//	}

	//	if (Combat->EquippedWeapon == nullptr)
	//	{
	//		//-1 == no EquippedWeapon yet. No need to reverse the icon animation
	//		EquipItemDelegate.Broadcast(-1, WeaponToEquip->GetSlotIndex());
	//	}
	//	else if (!bSwapping)
	//	{
	//		EquipItemDelegate.Broadcast(Combat->EquippedWeapon->GetSlotIndex(), WeaponToEquip->GetSlotIndex());
	//	}
	//	Combat->EquippedWeapon = WeaponToEquip;
	//	Combat->GetEquippedWeapon()->SetItemState(EItemState::EIS_Equipped);
	//}
}

void ABaseCharacter::DropWeapon()
{
	if (Combat->GetEquippedWeapon())
	{
		FDetachmentTransformRules DetachmentTransformRules(EDetachmentRule::KeepWorld, true);
		Combat->GetEquippedWeapon()->GetItemMesh()->DetachFromComponent(DetachmentTransformRules);

		Combat->GetEquippedWeapon()->SetItemState(EItemState::EIS_Falling);
		Combat->GetEquippedWeapon()->Dropped();
	}
}

void ABaseCharacter::SelectButtonPressed()
{
	if (CombatState != ECombatState::ECS_Unoccupied) return;
	if (Combat)
	{
		if (Combat->bHoldingTheFlag) return;
		if (Combat->CombatState == ECombatState::ECS_Unoccupied) ServerSelectButtonPressed();
		//bool bSwap = Combat->ShouldSwapWeapons() &&
		//	!HasAuthority() &&
		//	Combat->CombatState == ECombatState::ECS_Unoccupied &&
		//	TraceHitItem == nullptr;

		//if (bSwap)
		//{
		//	//PlaySwapMontage();
		//	Combat->CombatState = ECombatState::ECS_SwappingWeapons;
		//	//bFinishedSwapping = false;
		//}
	}
}

void ABaseCharacter::SelectButtonReleased()
{
}

void ABaseCharacter::ServerSelectButtonPressed_Implementation()
{
	if (Combat)
	{
		if (TraceHitItem)
		{
			AWeapon* TraceWeapon = Cast<AWeapon>(TraceHitItem);
			if (Combat->GetEquippedWeapon() == nullptr)
			{
				GetPickupItem(TraceWeapon);
				//Combat->EquipWeapon(TraceWeapon);
			}
			else
			{
				GetPickupItem(TraceWeapon);
			}
			TraceHitItem = nullptr;
			TraceHitItemLastFrame = nullptr;
		}
		else
		{
			if (Combat->GetEquippedWeapon())
			{
				Inventory[Combat->GetEquippedWeapon()->GetSlotIndex()] = nullptr;
				Combat->DropEquippedWeapon();
			}
		}
		//if (Combat->GetEquippedWeapon()==nullptr)
		//{
		//	//EquipWeapon(TraceWeapon, true);
		//	Combat->EquipWeapon(TraceWeapon);
		//}
		//else if (Combat->GetEquippedWeapon())
		//{
		//	if (GetEmptyInventorySlot() == -1)
		//	{
		//		SwapWeapon(TraceWeapon);
		//	}
		//	else
		//	{

		//	}
		//	//Combat->EquipWeapon(TraceWeapon);
		//	//Combat->SwapWeapons();
		//}
	}
}

void ABaseCharacter::ServerNumberButtonPressed_Implementation(int32 slotIndex)
{
	if (Combat)
	{
		if (Inventory.Num() > slotIndex)
		{
			if (Inventory[slotIndex])
			{
				auto Weapon = Cast<AWeapon>(Inventory[slotIndex]);
				if(Weapon)
					Combat->EquipWeapon(Weapon);
			}
		}
		//Combat->SwapWeapons();
	}
}

void ABaseCharacter::SpawDefaultWeapon()
{
	//BlasterGameMode = BlasterGameMode == nullptr ? GetWorld()->GetAuthGameMode<ABlasterGameMode>() : BlasterGameMode;
	UWorld* World = GetWorld();
	//if (BlasterGameMode && World && !bElimmed && DefaultWeaponClass)
	if (Combat)
	{
		if (World && Combat->DefaultWeaponClass)
		{
			AWeapon* StartingWeapon = World->SpawnActor<AWeapon>(Combat->DefaultWeaponClass);
			Combat->EquipWeapon(StartingWeapon);
		}
	}
}

void ABaseCharacter::SwapWeapon(AWeapon* WeaponToSwap)
{
	DropWeapon();
	EquipWeapon(WeaponToSwap, true);
	TraceHitItem = nullptr;
	TraceHitItemLastFrame = nullptr;
}

void ABaseCharacter::InitializeAmmoMap()
{
	AmmoMap.Add(EAmmoType::EAT_9mm, Starting9mmAmmo);
	AmmoMap.Add(EAmmoType::EAT_AR, StartingARAmmo);
}

void ABaseCharacter::PlayFireMontage(bool bAiming)
{
	//UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	//if (AnimInstance && HipFireMontage)
	//{
	//	AnimInstance->Montage_Play(HipFireMontage);
	//	AnimInstance->Montage_JumpToSection(FName("StartFire"));
	//}
	if (Combat == nullptr || Combat->EquippedWeapon == nullptr) return;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HipFireMontage)
	{
		AnimInstance->Montage_Play(HipFireMontage);
		FName SectionName;
		SectionName = bAiming ? FName("RifleAim") : FName("RifleHip");
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

void ABaseCharacter::PlayReloadMontage()
{
	if (Combat == nullptr || Combat->GetEquippedWeapon() == nullptr) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && ReloadMontage)
	{
		AnimInstance->Montage_Play(ReloadMontage);
		FName SectionName;

		switch (Combat->GetEquippedWeapon()->GetWeaponType())
		{
		case EWeaponType::EWT_AssaultRifle:
			SectionName = FName("Rifle");
			break;
		case EWeaponType::EWT_RocketLauncher:
			SectionName = FName("RocketLauncher");
			break;
		case EWeaponType::EWT_Pistol:
			SectionName = FName("Pistol");
			break;
		case EWeaponType::EWT_SubmachineGun:
			SectionName = FName("Pistol");
			break;
		case EWeaponType::EWT_Shotgun:
			SectionName = FName("Shotgun");
			break;
		case EWeaponType::EWT_SniperRifle:
			SectionName = FName("SniperRifle");
			break;
		case EWeaponType::EWT_GrenadeLauncher:
			SectionName = FName("GrenadeLauncher");
			break;
		}

		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

void ABaseCharacter::PlayElimMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && ElimMontage)
	{
		AnimInstance->Montage_Play(ElimMontage);
	}
}

void ABaseCharacter::PlaySwapMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && SwapMontage)
	{
		AnimInstance->Montage_Play(SwapMontage);
	}
}

void ABaseCharacter::CrouchButtonPressed()
{
	if (!GetCharacterMovement()->IsFalling())
	{
		bCrouching = !bCrouching;
	}
	if (bCrouching)
	{
		GetCharacterMovement()->MaxWalkSpeed = CrouchMovementSpeed;
		GetCharacterMovement()->GroundFriction = CrouchingGroundFriction;
		UnCrouch();
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;
		GetCharacterMovement()->GroundFriction = BaseGroundFriction;
		Crouch();
	}
}

void ABaseCharacter::InterpCapsuleHalfHeight(float DeltaTime)
{
	float TargetCapsuleHalfHeight{};
	if (bCrouching)
	{
		TargetCapsuleHalfHeight = CrouchingCapsuleHalfHeight;
	}
	else
	{
		TargetCapsuleHalfHeight = StandingCapsuleHalfHeight;
	}

	const float InterpHalfHeight{ FMath::FInterpTo(GetCapsuleComponent()->GetScaledCapsuleHalfHeight(),TargetCapsuleHalfHeight,DeltaTime,20.f) };

	const float DeltaCapsuleHalfHeight{ InterpHalfHeight - GetCapsuleComponent()->GetScaledCapsuleHalfHeight() };

	const FVector MeshOffset{ 0.f,0.f,-DeltaCapsuleHalfHeight };

	GetMesh()->AddLocalOffset(MeshOffset);

	GetCapsuleComponent()->SetCapsuleHalfHeight(InterpHalfHeight);
}

void ABaseCharacter::InitailizeInterpLocations()
{
	/*FInterpLocation WeaponLocation{ WeaponInterpComp, 0 };
	InterpLocations.Add(WeaponLocation);

	FInterpLocation InterpLoc1{ InterpComp1, 0 };
	InterpLocations.Add(InterpLoc1);

	FInterpLocation InterpLoc2{ InterpComp2, 0 };
	InterpLocations.Add(InterpLoc2);

	FInterpLocation InterpLoc3{ InterpComp3, 0 };
	InterpLocations.Add(InterpLoc3);

	FInterpLocation InterpLoc4{ InterpComp4, 0 };
	InterpLocations.Add(InterpLoc4);

	FInterpLocation InterpLoc5{ InterpComp5, 0 };
	InterpLocations.Add(InterpLoc5);

	FInterpLocation InterpLoc6{ InterpComp6, 0 };
	InterpLocations.Add(InterpLoc6);*/
}

void ABaseCharacter::OneKeyPressed()
{
	ServerNumberButtonPressed(0);
	//if(Combat->GetEquippedWeapon()==nullptr) return;
	//if (Combat->GetEquippedWeapon()->GetSlotIndex() == 1) return;
	//ExchangeInventoryItems(EquippedWeapon->GetSlotIndex(), 1);
}

void ABaseCharacter::TwoKeyPressed()
{
	ServerNumberButtonPressed(1);
	//if (Combat->GetEquippedWeapon() == nullptr) return;
	//if (Combat->GetEquippedWeapon()->GetSlotIndex() == 2) return;
	//ServerNumberButtonPressed(Combat->GetEquippedWeapon()->GetSlotIndex());
	//ExchangeInventoryItems(EquippedWeapon->GetSlotIndex(), 2);
}

void ABaseCharacter::ThreeKeyPressed()
{
	ServerNumberButtonPressed(2);
	//if (Combat->GetEquippedWeapon() == nullptr) return;
	//if (Combat->GetEquippedWeapon()->GetSlotIndex() == 3) return;
	//ServerNumberButtonPressed(Combat->GetEquippedWeapon()->GetSlotIndex());
	//ExchangeInventoryItems(EquippedWeapon->GetSlotIndex(), 3);
}

void ABaseCharacter::FourKeyPressed()
{
	ServerNumberButtonPressed(3);
	//if (Combat->GetEquippedWeapon() == nullptr) return;
	//if (Combat->GetEquippedWeapon()->GetSlotIndex() == 4) return;
	//ServerNumberButtonPressed(Combat->GetEquippedWeapon()->GetSlotIndex());
	//ExchangeInventoryItems(EquippedWeapon->GetSlotIndex(), 4);
}

void ABaseCharacter::FiveKeyPressed()
{
	ServerNumberButtonPressed(4);
	//if (Combat->GetEquippedWeapon() == nullptr) return;
	//if (Combat->GetEquippedWeapon()->GetSlotIndex() == 5) return;
	//ServerNumberButtonPressed(Combat->GetEquippedWeapon()->GetSlotIndex());
	//ExchangeInventoryItems(EquippedWeapon->GetSlotIndex(), 5);
}

void ABaseCharacter::Jump()
{
	if (bCrouching)
	{
		bCrouching = false;
		GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;
	}
	else
	{
		ACharacter::Jump();
	}
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//CameraInterpZoom(DeltaTime);
	SetLookRates();
	//CalculateCrosshairSpread(DeltaTime);
	RotateInPlace(DeltaTime);
	PollInit();
	if(HasAuthority())
		TraceForItems();
	InterpCapsuleHalfHeight(DeltaTime);

	if (CacheCharacterHeadWidget)
	{
		if(CacheCharacterAnimInstance)
			CacheCharacterHeadWidget->SetDisplayText(CacheCharacterAnimInstance->GetCharacterInfo());
		else
			CacheCharacterHeadWidget->SetDisplayText("CheckAnim");
	}
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis("TurnAtRate", this, &ABaseCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpAtRate", this, &ABaseCharacter::LookUpAtRate);
	PlayerInputComponent->BindAxis("Turn", this, &ABaseCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ABaseCharacter::LookUp);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this,&ABaseCharacter::FireButtonPressed);
	PlayerInputComponent->BindAction("Fire", IE_Released, this,&ABaseCharacter::FireButtonReleased);

	PlayerInputComponent->BindAction("Aiming", IE_Pressed, this,&ABaseCharacter::AimingButtonPressed);
	PlayerInputComponent->BindAction("Aiming", IE_Released, this,&ABaseCharacter::AimingButtonReleased);

	PlayerInputComponent->BindAction("Select", IE_Pressed, this, &ABaseCharacter::SelectButtonPressed);
	PlayerInputComponent->BindAction("Select", IE_Released, this, &ABaseCharacter::SelectButtonReleased);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABaseCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ABaseCharacter::StopJumping);

	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ABaseCharacter::CrouchButtonPressed);

	PlayerInputComponent->BindAction("OnEquipNum1", IE_Released, this, &ABaseCharacter::OneKeyPressed);
	PlayerInputComponent->BindAction("OnEquipNum2", IE_Released, this, &ABaseCharacter::TwoKeyPressed);
	PlayerInputComponent->BindAction("OnEquipNum3", IE_Released, this, &ABaseCharacter::ThreeKeyPressed);
	PlayerInputComponent->BindAction("OnEquipNum4", IE_Released, this, &ABaseCharacter::FourKeyPressed);
	PlayerInputComponent->BindAction("OnEquipNum5", IE_Released, this, &ABaseCharacter::FiveKeyPressed);

	PlayerInputComponent->BindAction("Q", EInputEvent::IE_Pressed, this, &ABaseCharacter::QKeyPressed);
	PlayerInputComponent->BindAction("E", EInputEvent::IE_Pressed, this, &ABaseCharacter::EKeyPressed);
	PlayerInputComponent->BindAction("R", EInputEvent::IE_Pressed, this, &ABaseCharacter::RKeyPressed);
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ABaseCharacter, TraceHitItem, COND_OwnerOnly);
	DOREPLIFETIME(ABaseCharacter, Inventory);
	DOREPLIFETIME(ABaseCharacter, Health);
	DOREPLIFETIME(ABaseCharacter, Shield);
}

void ABaseCharacter::OnRep_ReplicatedMovement()
{
	Super::OnRep_ReplicatedMovement();
	SimProxiesTurn();
	TimeSinceLastMovementReplication = 0.f;
}

void ABaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (Combat)
	{
		Combat->Character = this;
	}
	if (Buff)
	{
		Buff->Character = this;
		Buff->SetInitialSpeeds(
			GetCharacterMovement()->MaxWalkSpeed,
			GetCharacterMovement()->MaxWalkSpeedCrouched
		);
		Buff->SetInitialJumpVelocity(GetCharacterMovement()->JumpZVelocity);
	}
	if (LagCompensation)
	{
		LagCompensation->Character = this;
		if (Controller)
		{
			LagCompensation->Controller = Cast<AInGamePlayerController>(Controller);
		}
	}
}

void ABaseCharacter::FinishReloading()
{
	CombatState = ECombatState::ECS_Unoccupied;

	if (bAimingButtonPressed)
	{
		if (Combat)
		{
			Combat->SetAiming(true);
		}
		//Aim();
	}

	const auto AmmoType{ Combat->GetEquippedWeapon()->GetAmmoType() };

	if (AmmoMap.Contains(AmmoType))
	{
		int32 CarriedAmmo = AmmoMap[AmmoType];

		const int32 MagEmptySpace = Combat->GetEquippedWeapon()->GetMagazineCapacity() - Combat->GetEquippedWeapon()->GetAmmo();

		if (MagEmptySpace > CarriedAmmo)
		{
			Combat->GetEquippedWeapon()->ReloadAmmo(CarriedAmmo);
			CarriedAmmo = 0;
			AmmoMap.Add(AmmoType, CarriedAmmo);
		}
		else
		{
			Combat->GetEquippedWeapon()->ReloadAmmo(MagEmptySpace);
			CarriedAmmo -= MagEmptySpace;
			AmmoMap.Add(AmmoType, CarriedAmmo);
		}
	}
}

void ABaseCharacter::FinishEquipping()
{
	CombatState = ECombatState::ECS_Unoccupied;
}

void ABaseCharacter::OnRep_TraceItem(AItem* LastTraceHitItem)
{
	if (TraceHitItem)
	{
		TraceHitItem->GetPickupWidget()->SetVisibility(true);
	}
	if (LastTraceHitItem)
	{
		LastTraceHitItem->GetPickupWidget()->SetVisibility(false);
	}
}

void ABaseCharacter::IncrementOverlappedItemCount(int8 Amount)
{
	if (OverlappedItemCount + Amount <= 0)
	{
		OverlappedItemCount = 0;
		bShouldTraceForItems = false;
	}
	else
	{
		OverlappedItemCount += Amount;
		bShouldTraceForItems = true;
	}
}

void ABaseCharacter::GetPickupItem(AItem* Item)
{
	//Item->PlayEquipSound();

	//if (Item->GetEquipSound())
	//{
	//	//UGameplayStatics::PlaySound2D(this, Item->GetEquipSound());
	//}
	int32 slotIndex = GetEmptyInventorySlot();
	if (slotIndex != -1)
	{
		Item->SetSlotIndex(slotIndex);
		if (slotIndex < Inventory.Num())
		{
			Inventory[slotIndex] = Item;
		}
		else
		{
			Inventory.Add(Item);
		}
		Item->SetItemState(EItemState::EIS_PickedUp);
	}
	else
	{
		if (Inventory[Combat->GetEquippedWeapon()->GetSlotIndex()])
		{

		}
	}

	if (Combat->GetEquippedWeapon() == nullptr)
	{
		auto Weapon = Cast<AWeapon>(Item);
		if (Weapon)
		{
			Combat->EquipWeapon(Weapon);
		}
	}

	auto Ammo = Cast<AAmmo>(Item);

	if (Ammo)
	{
		//PickupAmmo(Ammo);
	}
}

void ABaseCharacter::ResetPickupSoundTimer()
{
	bShouldPlayPickupSound = true;
}

void ABaseCharacter::ResetEquipSoundTimer()
{
	bShouldPlayEquipSound = true;
}
FInterpLocation ABaseCharacter::GetInterpLocation(int32 Index)
{
	if (Index <= InterpLocations.Num())
	{
		return InterpLocations[Index];
	}
	return FInterpLocation();
}
int32 ABaseCharacter::GetInterpLocationIndex()
{
	int32 LowestIndex = 1;
	int32 LowestCount = INT_MAX;

	for (int32 i = 1; i < InterpLocations.Num(); ++i)
	{
		if (InterpLocations[i].ItemCount < LowestCount)
		{
			LowestIndex = i;
			LowestCount = InterpLocations[i].ItemCount;
		}
	}

	return LowestIndex;
}

int32 ABaseCharacter::GetEmptyInventorySlot()
{
	for (int32 i = 0; i < Inventory.Num(); ++i)
	{
		if (Inventory[i] == nullptr)
		{
			return i;
		}
	}
	if (Inventory.Num() < INVENTORY_CAPACITY)
	{
		return Inventory.Num();
	}
	return -1;
}
void ABaseCharacter::HighlightInventorySlot()
{
	const int32 EmptySlot{ GetEmptyInventorySlot() };
	HighlightIconDelegate.Broadcast(EmptySlot, true);
	HighlightedSlot = EmptySlot;
}

void ABaseCharacter::SetSpawnPoint()
{
	if (HasAuthority() )//&& BasePlayerState->GetTeam() != ETeam::ET_NoTeam)
	{
		//TArray<AActor*> PlayerStarts;
		//UGameplayStatics::GetAllActorsOfClass(this, ATeamPlayerStart::StaticClass(), PlayerStarts);
		//TArray<ATeamPlayerStart*> TeamPlayerStarts;
		//for (auto Start : PlayerStarts)
		//{
		//	ATeamPlayerStart* TeamStart = Cast<ATeamPlayerStart>(Start);
		//	if (TeamStart && TeamStart->Team == BasePlayerState->GetTeam())
		//	{
		//		TeamPlayerStarts.Add(TeamStart);
		//	}
		//}
		//if (TeamPlayerStarts.Num() > 0)
		//{
		//	ATeamPlayerStart* ChosenPlayerStart = TeamPlayerStarts[FMath::RandRange(0, TeamPlayerStarts.Num() - 1)];
		//	SetActorLocationAndRotation(
		//		ChosenPlayerStart->GetActorLocation(),
		//		ChosenPlayerStart->GetActorRotation()
		//	);
		//}
	}
}

void ABaseCharacter::OnPlayerStateInitialized()
{
	//BasePlayerState->AddToScore(0.f);
	//BasePlayerState->AddToDefeats(0);
	//SetTeamColor(BasePlayerState->GetTeam());
	//SetSpawnPoint();
}

void ABaseCharacter::PlayHitReactMontage()
{
	if (Combat == nullptr || Combat->GetEquippedWeapon() == nullptr) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		FName SectionName("FromFront");
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

void ABaseCharacter::QKeyPressed()
{
}

void ABaseCharacter::EKeyPressed()
{
}

void ABaseCharacter::RKeyPressed()
{
}

void ABaseCharacter::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser)
{
	BattleGameMode = BattleGameMode == nullptr ? GetWorld()->GetAuthGameMode<ABattleGameMode>() : BattleGameMode;
	if (BattleGameMode == nullptr) return;
	Damage = BattleGameMode->CalculateDamage(InstigatorController, Controller, Damage);

	float DamageToHealth = Damage;
	if (Shield > 0.f)
	{
		if (Shield >= Damage)
		{
			Shield = FMath::Clamp(Shield - Damage, 0.f, MaxShield);
			DamageToHealth = 0.f;
		}
		else
		{
			DamageToHealth = FMath::Clamp(DamageToHealth - Shield, 0.f, Damage);
			Shield = 0.f;
		}
	}

	Health = FMath::Clamp(Health - DamageToHealth, 0.f, MaxHealth);

	UpdateHUDHealth();
	UpdateHUDShield();
	PlayHitReactMontage();

	if (Health == 0.f)
	{
		if (BattleGameMode)
		{
			BasePlayerController = BasePlayerController == nullptr ? Cast<AInGamePlayerController>(Controller) : BasePlayerController;
			AInGamePlayerController* AttackerController = Cast<AInGamePlayerController>(InstigatorController);
			BattleGameMode->PlayerEliminated(this, BasePlayerController, AttackerController);
		}
	}
}

void ABaseCharacter::UnHighlightInventorySlot()
{
	HighlightIconDelegate.Broadcast(HighlightedSlot, false);
	HighlightedSlot = -1;
}

void ABaseCharacter::IncrementInterpLocItemCount(int32 Index, int32 Amount)
{
	if (Amount < -1 || Amount > 1) return;
	if (InterpLocations.Num() >= Index)
	{
		InterpLocations[Index].ItemCount += Amount;
	}
}

bool ABaseCharacter::TraceScreenCrosshairCollision(FHitResult& OutHitResult, FVector& OutHitLocation)
{
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	FVector2D CrosshairLocation(ViewportSize.X * 0.5f, ViewportSize.Y * 0.5f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	bool bScreenToWorld = false;

	if (HasAuthority())
	{
		if (CharacterCamera)
		{
			CrosshairWorldPosition = CharacterCamera->GetComponentToWorld().GetLocation();
			CrosshairWorldDirection = CharacterCamera->GetForwardVector();
			bScreenToWorld = true;
		}
		else
		{
			CHelpers::CheckNullComponent<UCameraComponent>(this, &CharacterCamera);
			CrosshairWorldPosition = CharacterCamera->GetComponentToWorld().GetLocation();
			CrosshairWorldDirection = CharacterCamera->GetForwardVector();
			bScreenToWorld = true;
		}
	}
	else
	{
		bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0), CrosshairLocation, CrosshairWorldPosition, CrosshairWorldDirection);
	}
	//bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0), CrosshairLocation, CrosshairWorldPosition, CrosshairWorldDirection);
	if (bScreenToWorld)
	{
		FVector Start{ CrosshairWorldPosition };
		//float DistanceToCharacter = (GetActorLocation() - Start).Size();
		//Start += CrosshairWorldDirection * (DistanceToCharacter + 100.f);
		const FVector End{ Start + CrosshairWorldDirection * 50'000.f };
		OutHitLocation = End;
		GetWorld()->LineTraceSingleByChannel(OutHitResult, Start, End, ECollisionChannel::ECC_Visibility);
		if (OutHitResult.bBlockingHit)
		{
			//DrawDebugLine(GetWorld(), Start, End, FColor::Black, false, 2.f);
			//DrawDebugPoint(GetWorld(), OutHitResult.Location, 5.f, FColor::Black, false, 2.f);
			OutHitLocation = OutHitResult.Location;
			return true;
		}
	}
	return false;
}

void ABaseCharacter::UpdateHUDHealth()
{
	BasePlayerController = BasePlayerController == nullptr ? Cast<AInGamePlayerController>(Controller) : BasePlayerController;
	if (BasePlayerController)
	{
		BasePlayerController->SetHUDHealth(Health, MaxHealth);
	}
}

void ABaseCharacter::UpdateHUDShield()
{
	BasePlayerController = BasePlayerController == nullptr ? Cast<AInGamePlayerController>(Controller) : BasePlayerController;
	if (BasePlayerController)
	{
		BasePlayerController->SetHUDShield(Shield, MaxShield);
	}
}

void ABaseCharacter::UpdateHUDAmmo()
{
	BasePlayerController = BasePlayerController == nullptr ? Cast<AInGamePlayerController>(Controller) : BasePlayerController;
	if (BasePlayerController)
	{
	}
}

void ABaseCharacter::StartPickupSoundTimer()
{
	bShouldPlayPickupSound = false;
	GetWorldTimerManager().SetTimer(PickupSoundTimer, this, &ABaseCharacter::ResetPickupSoundTimer, PickupSoundResetTime);
}

void ABaseCharacter::StartEquipSoundTimer()
{
	bShouldPlayEquipSound = false;
	GetWorldTimerManager().SetTimer(EquipSoundTimer, this, &ABaseCharacter::ResetEquipSoundTimer, EquipSoundResetTime);
}

void ABaseCharacter::OnRep_Inventory()
{
	/*GetCharacterHeadWidget()->SetDisplayText(FString::Printf(TEXT("%d"), Inventory[Inventory.Num()-1]->GetSlotIndex()));
	if (BasePlayerController)
	{
		for (AItem* item : Inventory)
		{
			BasePlayerController->SetHUDInventorySlot(item);
		}
	}*/
	//	GetCharacterHeadWidget()->SetDisplayText("OnRep_Inventory");
	// 
	
	//현재 어떤 아이템을 사용하는지 확인
	if (Combat->GetEquippedWeapon() == nullptr)
	{

		//-1 == no EquippedWeapon yet. No need to reverse the icon animation
		//EquipItemDelegate.Broadcast(-1, WeaponToEquip->GetSlotIndex());
	}
	//else if (!bSwapping)
	//{
	//	//EquipItemDelegate.Broadcast(Combat->EquippedWeapon->GetSlotIndex(), WeaponToEquip->GetSlotIndex());
	//}
}

void ABaseCharacter::PollInit()
{
	if (BasePlayerState == nullptr)
	{
		BasePlayerState = GetPlayerState<AInGamePlayerState>();
		if (BasePlayerState)
		{
			OnPlayerStateInitialized();

			//AInGamePlayerState* TempPlayerState = Cast<AInGameState>(UGameplayStatics::GetGameState(this));

			//if (TempPlayerState && TempPlayerState->TopScoringPlayers.Contains(PlayerState))
			//{
			//	//MulticastGainedTheLead();
			//}
		}
	}
	if (BasePlayerController == nullptr)
	{
		BasePlayerController = BasePlayerController == nullptr ? Cast<AInGamePlayerController>(Controller) : BasePlayerController;
		if (BasePlayerController)
		{
			//SpawDefaultWeapon();
			UpdateHUDAmmo();
			UpdateHUDHealth();
			UpdateHUDShield();
		}
	}
}

void ABaseCharacter::RotateInPlace(float DeltaTime)
{
	//if (Combat) //&& Combat->bHoldingTheFlag)
	//{
	//	bUseControllerRotationYaw = false;
	//	GetCharacterMovement()->bOrientRotationToMovement = true;
	//	TurningInPlace = ETurningInPlace::ETIP_NotTurning;
	//	return;
	//}
	if (Combat && Combat->GetEquippedWeapon()) GetCharacterMovement()->bOrientRotationToMovement = false;
	if (Combat && Combat->GetEquippedWeapon()) bUseControllerRotationYaw = true;
	//if (bDisableGameplay)
	//{
	//	bUseControllerRotationYaw = false;
	//	TurningInPlace = ETurningInPlace::ETIP_NotTurning;
	//	return;
	//}
	if (GetLocalRole() > ENetRole::ROLE_SimulatedProxy && IsLocallyControlled())
	{
		AimOffset(DeltaTime);
	}
	else
	{
		TimeSinceLastMovementReplication += DeltaTime;
		if (TimeSinceLastMovementReplication > 0.25f)
		{
			OnRep_ReplicatedMovement();
		}
		CalculateAO_Pitch();
	}
}

void ABaseCharacter::OnRep_Health(float LastHealth)
{
	UpdateHUDHealth();
	if (Health < LastHealth)
	{
		//PlayHitReactMontage();
	}
}

void ABaseCharacter::OnRep_Shield(float LastShield)
{
	UpdateHUDShield();
	if (Shield < LastShield)
	{
		//PlayHitReactMontage();
	}
}

bool ABaseCharacter::IsAiming()
{
	return (Combat && Combat->GetAiming());
}

void ABaseCharacter::SetFindCamera()
{
	CHelpers::CheckNullComponent<UCameraComponent>(this, &CharacterCamera);
}

bool ABaseCharacter::IsLocallyReloading()
{
	if (Combat == nullptr) return false;
	return Combat->bLocallyReloading;
}