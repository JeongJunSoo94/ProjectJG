#include "WorldObjects/Item/Weapon.h"
#include "Global.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Net/UnrealNetwork.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "BaseSystem/InGamePlayerController.h"
#include "Character/BaseCharacter.h"
#include "Character/Components/CombatComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Bullet/Projectile.h"
#include "Bullet/Case.h"
#include "Sound/SoundCue.h"

AWeapon::AWeapon() :
	ThrowWeaponTime(0.7f),
	bFalling(false),
	Ammo(30),
	MagazineCapacity(30),
	WeaponType(EWeaponType::EWT_SubmachineGun),
	ReloadMontageSection(FName(TEXT("SubMachineGun"))),
	ClipBoneName(TEXT("smg_clip")),
	SlideDisplacement(0.f),
	SlideDisplacementTime(0.2f),
	bMovingSlide(false),
	MaxSlideDisplacement(4.f),
	MaxRecoilRotation(20.f)
{
	PrimaryActorTick.bCanEverTick = true;
}
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetItemState() == EItemState::EIS_Falling && bFalling)
	{
		const FRotator MeshRotation{ 0.f,GetItemMesh()->GetComponentRotation().Yaw,0.f };
		GetItemMesh()->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);
	}
	UpdateSlideDisplacement();
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AWeapon, WeaponState);
	DOREPLIFETIME_CONDITION(AWeapon, bUseServerSideRewind, COND_OwnerOnly);
}

void AWeapon::SetHUDAmmo()
{
	BaseOwnerCharacter = BaseOwnerCharacter == nullptr ? Cast<ABaseCharacter>(GetOwner()) : BaseOwnerCharacter;
	if (BaseOwnerCharacter)
	{
		BaseOwnerController = BaseOwnerController == nullptr ? Cast<AInGamePlayerController>(BaseOwnerCharacter->Controller) : BaseOwnerController;
		if (BaseOwnerController)
		{
			BaseOwnerController->SetHUDWeaponAmmo(Ammo);
		}
	}
}

void AWeapon::OnRep_Owner()
{
	Super::OnRep_Owner();
	if (Owner == nullptr)
	{
		BaseOwnerCharacter = nullptr;
		BaseOwnerController = nullptr;
	}
	else
	{
		BaseOwnerCharacter = BaseOwnerCharacter == nullptr ? Cast<ABaseCharacter>(Owner) : BaseOwnerCharacter;
		if (BaseOwnerCharacter && BaseOwnerCharacter->GetCombatComp()->GetEquippedWeapon() && BaseOwnerCharacter->GetCombatComp()->GetEquippedWeapon() == this)
		{
			SetHUDAmmo();
		}
	}
}

//void AWeapon::OnRep_WeaponState()
//{
//	Clog::Log("OnRep_WeaponState");
//	OnWeaponStateSet();
//}

void AWeapon::SetItemProperties(EItemState State)
{
	Super::SetItemProperties(State);
}

//void AWeapon::OnEquipped()
//{
//	Clog::Log("OnEquipped");
//	//ShowPickupWidget(false);
//	//AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//	//WeaponMesh->SetSimulatePhysics(false);
//	//WeaponMesh->SetEnableGravity(false);
//	//WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//	//if (WeaponType == EWeaponType::EWT_SubmachineGun)
//	//{
//	//	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
//	//	WeaponMesh->SetEnableGravity(true);
//	//	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
//	//}
//	//EnableCustomDepth(false);
//	/*BlasterOwnerCharacter = BlasterOwnerCharacter == nullptr ? Cast<ABlasterCharacter>(GetOwner()) : BlasterOwnerCharacter;
//	if (BlasterOwnerCharacter && bUseServerSideRewind)
//	{
//		BlasterOwnerController = BlasterOwnerController == nullptr ? Cast<ABlasterPlayerController>(BlasterOwnerCharacter->Controller) : BlasterOwnerController;
//		if (BlasterOwnerController && HasAuthority() && !BlasterOwnerController->HighPingDelegate.IsBound())
//		{
//			BlasterOwnerController->HighPingDelegate.AddDynamic(this, &AWeapon::OnPingTooHigh);
//		}
//	}*/
//}

//void AWeapon::SetWeaponState(EWeaponState State)
//{
//	Clog::Log("SetWeaponState");
//	WeaponState = State;
//	OnWeaponStateSet();
//}

//void AWeapon::OnWeaponStateSet()
//{
//	Clog::Log("OnWeaponStateSet");
//	switch (WeaponState)
//	{
//	case EWeaponState::EWS_Equipped:
//		//OnEquipped();
//		break;
//	case EWeaponState::EWS_EquippedSecondary:
//		//OnEquippedSecondary();
//		break;
//	case EWeaponState::EWS_Dropped:
//		//OnDropped();
//		break;
//	}
//}

void AWeapon::Dropped()
{
	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
	GetItemMesh()->DetachFromComponent(DetachRules);
	SetOwner(nullptr);
	BaseOwnerCharacter = nullptr;
	BaseOwnerController = nullptr;
	bFalling = true;
	GetWorldTimerManager().SetTimer(ThrowWeaponTimer, this, &AWeapon::StopFalling, ThrowWeaponTime);
	EnableGlowMaterial();
	//ThrowWeapon();
	//Super::Dropped();
}

void AWeapon::ThrowWeapon()
{
	//if (HasAuthority())
	{
		FRotator MeshRotation{ 0.f,GetItemMesh()->GetComponentRotation().Yaw,0.f };
		GetItemMesh()->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);

		const FVector MeshForward{ GetItemMesh()->GetForwardVector() };
		const FVector MeshRight{ GetItemMesh()->GetRightVector() };
		FVector ImpulseDirection = MeshRight.RotateAngleAxis(-20.f, MeshForward);

		float RandomRotation{ 3.f };
		ImpulseDirection = ImpulseDirection.RotateAngleAxis(RandomRotation, FVector(0.f, 0.f, 1.f));
		ImpulseDirection *= 2000.f;
		GetItemMesh()->AddImpulse(ImpulseDirection);

		bFalling = true;
		GetWorldTimerManager().SetTimer(ThrowWeaponTimer, this, &AWeapon::StopFalling, ThrowWeaponTime);
	}
	//SetOwner(nullptr);
	//EnableGlowMaterial();
	//SetItemState(EItemState::EIS_Pickup);
	//FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
	//GetItemMesh()->DetachFromComponent(DetachRules);
	//SetOwner(nullptr);
	//BaseOwnerCharacter = nullptr;
	//BaseOwnerController = nullptr;
}

void AWeapon::StopFalling()
{
	bFalling = false;
	SetItemState(EItemState::EIS_Pickup);
	StartPulseTimer();
}

void AWeapon::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	const FString WeaponTablePath(TEXT("DataTable'/Game/Developers/JJS/Weapons/WeaponsDataTable/WeaponDataTable.WeaponDataTable'"));
	UDataTable* WeaponTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *WeaponTablePath));

	if (WeaponTableObject)
	{
		FWeaponDataTable* WeaponDataRow = nullptr;
		switch (WeaponType)
		{
		case EWeaponType::EWT_SubmachineGun:
			WeaponDataRow = WeaponTableObject->FindRow<FWeaponDataTable>(FName("SubmachineGun"), TEXT(""));
			break;
		case EWeaponType::EWT_AssaultRifle:
			WeaponDataRow = WeaponTableObject->FindRow<FWeaponDataTable>(FName("AssaultRifle"), TEXT(""));
			break;
		case EWeaponType::EWT_Pistol:
			WeaponDataRow = WeaponTableObject->FindRow<FWeaponDataTable>(FName("Pistol"), TEXT(""));
			break;
		case EWeaponType::EWT_RocketLauncher:
			WeaponDataRow = WeaponTableObject->FindRow<FWeaponDataTable>(FName("RocketLauncher"), TEXT(""));
			break;
		case EWeaponType::EWT_Shotgun:
			WeaponDataRow = WeaponTableObject->FindRow<FWeaponDataTable>(FName("Shotgun"), TEXT(""));
			break;
		case EWeaponType::EWT_SniperRifle:
			WeaponDataRow = WeaponTableObject->FindRow<FWeaponDataTable>(FName("SniperRifle"), TEXT(""));
			break;
		case EWeaponType::EWT_GrenadeLauncher:
			WeaponDataRow = WeaponTableObject->FindRow<FWeaponDataTable>(FName("GrenadeLauncher"), TEXT(""));
			break;
		}

		if (WeaponDataRow)
		{
			//AmmoType = WeaponDataRow->AmmoType;
			Ammo = WeaponDataRow->WeaponAmmo;
			MagazineCapacity = WeaponDataRow->MagazingCapacity;

			Damage = WeaponDataRow->Damage*GetNumberOfStars();
			HeadShotDamage = WeaponDataRow->HeadDamage * GetNumberOfStars();

			SetPickupSound(WeaponDataRow->PickupSound);
			SetEquipSound(WeaponDataRow->EquipSound);
			GetItemMesh()->SetSkeletalMesh(WeaponDataRow->ItemMesh);
			SetItemName(WeaponDataRow->ItemName);
			SetIconItem(WeaponDataRow->InventoryIcon);
			SetAmmoIcon(WeaponDataRow->AmmoIcon);

			SetMaterialInstance(WeaponDataRow->MaterialInstance);
			PreviousMaterialIndex = GetMaterialIndex();
			GetItemMesh()->SetMaterial(PreviousMaterialIndex, nullptr);
			SetMaterialIndex(WeaponDataRow->MaterialIndex);
			SetClipBoneName(WeaponDataRow->ClipBoneName);
			SetReloadMontageSection(WeaponDataRow->ReloadMontageSection);
			GetItemMesh()->SetAnimInstanceClass(WeaponDataRow->AnimBP);
			SlideDisplacementCurve = WeaponDataRow->SlideDisplacementCurve;
			SlideDisplacementTime = WeaponDataRow->FireDelay * 2.0f;
			MaxSlideDisplacement = WeaponDataRow->MaxSlideDisplacement;
			CrosshairsCenter = WeaponDataRow->CrosshairsCenter;
			CrosshairsLeft = WeaponDataRow->CrosshairsLeft;
			CrosshairsRight = WeaponDataRow->CrosshairsRight;
			CrosshairsBottom = WeaponDataRow->CrosshairsBottom;
			CrosshairsTop = WeaponDataRow->CrosshairsTop;
			FireDelay = WeaponDataRow->FireDelay;
			BoneToHide = WeaponDataRow->BoneToHide;
			//
			CaseClass = WeaponDataRow->CaseClass;
			CaseParticle = WeaponDataRow->CaseParticle;

			MuzzleFlash = WeaponDataRow->MuzzleFlash;
			FireSound = WeaponDataRow->FireSound;
			MuzzleSocketName = WeaponDataRow->MuzzleSocketName;
			AmmoEjectSocketName = WeaponDataRow->AmmoEjectSocketName;
			MainHandSocketName = WeaponDataRow->MainHandSocketName;
			SubHandSocketName = WeaponDataRow->SubHandSocketName;
			CharacterAttachRightHandSocketName = WeaponDataRow->CharacterAttachRightHandSocketName;
			ZoomedFOV = WeaponDataRow->ZoomedFOV;
			ZoomInterpSpeed = WeaponDataRow->ZoomInterpSpeed;
		}
		if (GetMaterialInstance())
		{
			SetDynamicMaterialInstance(UMaterialInstanceDynamic::Create(GetMaterialInstance(), this));
			GetDynamicMaterialInstance()->SetVectorParameterValue(TEXT("FresnelColor"), GetGlowColor());
			GetItemMesh()->SetMaterial(GetMaterialIndex(), GetDynamicMaterialInstance());

			EnableGlowMaterial();
		}
	}
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	if (BoneToHide != FName(""))
	{
		GetItemMesh()->HideBoneByName(BoneToHide, EPhysBodyOp::PBO_None);
	}
}

void AWeapon::FinishMovingSlide()
{
	bMovingSlide = false;
}

void AWeapon::UpdateSlideDisplacement()
{
	if (SlideDisplacementCurve && bMovingSlide)
	{
		const float ElapsedTime{ GetWorldTimerManager().GetTimerElapsed(SlideTimer) };
		const float CurveValue{ SlideDisplacementCurve->GetFloatValue(ElapsedTime) };
		SlideDisplacement = CurveValue * MaxSlideDisplacement;
		RecoilRotation = CurveValue * MaxRecoilRotation;
	}
}

void AWeapon::DecrementAmmo()
{
	Ammo = FMath::Clamp(Ammo - 1, 0, MagazineCapacity);
	SetHUDAmmo();
	if (HasAuthority())
	{
		ClientUpdateAmmo(Ammo);
	}
	else
	{
		++Sequence;
	}
}

void AWeapon::ClientUpdateAmmo_Implementation(int32 ServerAmmo)
{
	if (HasAuthority()) return;
	Ammo = ServerAmmo;
	--Sequence;
	Ammo -= Sequence;
	SetHUDAmmo();
}


bool AWeapon::IsEmpty()
{
	return Ammo <= 0;
}

void AWeapon::AddAmmo(int32 AmmoToAdd)
{
	Ammo = FMath::Clamp(Ammo + AmmoToAdd, 0, MagazineCapacity);
	SetHUDAmmo();
	ClientAddAmmo(AmmoToAdd);
}

void AWeapon::ReloadAmmo(int32 Amount)
{
	//checkf(Ammo + Amount <= MagazineCapacity, TEXT("Attempted to reload with more than magazine capacity!"));
	//Ammo += Amount;

	Ammo = FMath::Clamp(Ammo + Amount, 0, MagazineCapacity);
	SetHUDAmmo();
	ClientAddAmmo(Amount);
}

void AWeapon::StartSlideTimer()
{
	bMovingSlide = true;
	GetWorldTimerManager().SetTimer(
		SlideTimer,
		this,
		&AWeapon::FinishMovingSlide,
		SlideDisplacementTime
	);
}

void AWeapon::ClientAddAmmo_Implementation(int32 Amount)
{
	if (HasAuthority()) return;
	Ammo = FMath::Clamp(Ammo + Amount, 0, MagazineCapacity);
	BaseOwnerCharacter = BaseOwnerCharacter == nullptr ? Cast<ABaseCharacter>(Owner) : BaseOwnerCharacter;
	//BlasterOwnerCharacter = BlasterOwnerCharacter == nullptr ? Cast<ABlasterCharacter>(GetOwner()) : BlasterOwnerCharacter;
	if (BaseOwnerCharacter && BaseOwnerCharacter->GetCombatComp())// && IsFull())
	{
		//BaseOwnerCharacter->GetCombat()->JumpToShotgunEnd();
	}
	SetHUDAmmo();
}

bool AWeapon::ClipIsFull()
{
	return Ammo >= MagazineCapacity;
}

FVector AWeapon::GetBeamTraceDirection(const FVector& HitTarget)
{
	const USkeletalMeshSocket* MuzzleFlashSocket = GetItemMesh()->GetSocketByName(MuzzleSocketName);
	if (MuzzleFlashSocket == nullptr) return FVector();

	const FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetItemMesh());
	const FVector TraceStart = SocketTransform.GetLocation();

	const FVector ToTargetNormalized = (HitTarget - TraceStart).GetSafeNormal();
	const FVector SphereCenter = TraceStart + ToTargetNormalized * DistanceToSphere;
	const FVector RandVec = UKismetMathLibrary::RandomUnitVector() * FMath::FRandRange(0.f, SphereRadius);
	const FVector EndLoc = SphereCenter + RandVec;
	const FVector ToEndLoc = EndLoc - TraceStart;

	return FVector(TraceStart + ToEndLoc * TRACE_LENGTH / ToEndLoc.Size());
}

bool AWeapon::GetBeamTraceDirection(const FVector& OutStartPosition, FVector& OutHitLocation)
{
	// Check for crosshair trace hit
	FHitResult CrosshairHitResult;
	//bool bCrosshairHit;// = TraceScreenCrosshairCollision(CrosshairHitResult, OutHitLocation);

	//if (bCrosshairHit)
	//{
	//	// Tentative beam location - still need to trace from gun
	//	OutHitLocation = CrosshairHitResult.Location;
	//}
	//else // no crosshair trace hit
	//{
	//	// OutBeamLocation is the End location for the line trace
	//}
	FHitResult WeaponHitResult;
	// Perform a second trace, this time from the gun barrel
	const FVector WeaponTraceStart{ OutStartPosition };
	const FVector StartToEnd{ OutHitLocation - WeaponTraceStart };
	const FVector WeaponTraceEnd{ OutStartPosition + StartToEnd * 1.25f };
	GetWorld()->LineTraceSingleByChannel(WeaponHitResult, WeaponTraceStart, WeaponTraceEnd, ECollisionChannel::ECC_Visibility);
	if (WeaponHitResult.bBlockingHit) // object between barrel and BeamEndPoint?
	{
		DrawDebugLine(GetWorld(), WeaponTraceStart, WeaponTraceEnd, FColor::Blue, false, 2.f);
		DrawDebugPoint(GetWorld(), WeaponHitResult.Location, 5.f, FColor::Blue, false, 2.f);
		OutHitLocation = WeaponHitResult.Location;
		return false;
	}

	return true;
}

//서버 없는 클라전용
void AWeapon::SendBullet()
{
	//const USkeletalMeshSocket* BarrelSocket = GetMesh()->GetSocketByName("BarrelSocket");
	const USkeletalMeshSocket* BarrelSocket = GetItemMesh()->GetSocketByName("BarrelSocket");

	if (BarrelSocket)
	{
		const FTransform SocketTransform = BarrelSocket->GetSocketTransform(GetItemMesh());
		DrawDebugPoint(GetWorld(), SocketTransform.GetLocation(), 5.f, FColor::Red, false, 2.f);
		//new
		/*if (MuzzleFlash)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, SocketTransform);
		}

		FVector BeamEnd;
		bool bBeamEnd = GetBeamTraceDirection(SocketTransform.GetLocation(), BeamEnd);
		if (bBeamEnd)
		{
			if (ImpactParticles)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, BeamEnd);
			}

			if (BeamParticles)
			{
				UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BeamParticles, SocketTransform);
				if (Beam)
				{
					Beam->SetVectorParameter(FName("Target"), BeamEnd);
				}
			}
		}*/
	}
	DecrementAmmo();
}

void AWeapon::Fire(const FVector& HitTarget)
{
	if (FireAnimation)
	{
		GetItemMesh()->PlayAnimation(FireAnimation, false);
	}
	if (CaseClass || CaseParticle)
	{
		const USkeletalMeshSocket* AmmoEjectSocket = GetItemMesh()->GetSocketByName(AmmoEjectSocketName);
		if (AmmoEjectSocket)
		{
			FTransform SocketTransform = AmmoEjectSocket->GetSocketTransform(GetItemMesh());

			UWorld* World = GetWorld();
			if (World)
			{
				if (CaseClass)
				{
					World->SpawnActor<ACase>(
						CaseClass,
						SocketTransform.GetLocation(),
						SocketTransform.GetRotation().Rotator()
					);
				}
				else if (CaseParticle)
				{
					UGameplayStatics::SpawnEmitterAtLocation(
						GetWorld(),
						CaseParticle,
						SocketTransform
					);
				}
			}
		}
	}
	const USkeletalMeshSocket* MuzzleFlashSocket = GetItemMesh()->GetSocketByName(GetMuzzleSocketName());
	if (MuzzleFlashSocket)
	{
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetItemMesh());
		if (MuzzleFlash)
		{
			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				MuzzleFlash,
				SocketTransform
			);
		}
	}

	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			FireSound,
			GetActorLocation()
		);
	}

	DecrementAmmo();
	if(SlideDisplacementCurve)
		StartSlideTimer();
}