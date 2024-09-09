#include "WorldObjects/Item/HitScanWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Character/BaseCharacter.h"
#include "BaseSystem/InGamePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "WorldObjects/Item/WeaponType.h"
//#include "Blaster/BlasterComponents/LagCompensationComponent.h"

void AHitScanWeapon::Fire(const FVector& HitTarget)
{
	Super::Fire(HitTarget);

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return;
	AController* InstigatorController = OwnerPawn->GetController();

	const USkeletalMeshSocket* MuzzleFlashSocket = GetItemMesh()->GetSocketByName(GetMuzzleSoketName());
	if (MuzzleFlashSocket)
	{
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetItemMesh());
		FVector Start = SocketTransform.GetLocation();

		FHitResult FireHit;
		WeaponTraceHit(Start, HitTarget, FireHit);

		ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(FireHit.GetActor());
		if (BaseCharacter && InstigatorController)
		{
			bool bCauseAuthDamage = !bUseServerSideRewind || OwnerPawn->IsLocallyControlled();
			if (HasAuthority() && bCauseAuthDamage)
			{
				const float DamageToCause = FireHit.BoneName.ToString() == FString("head") ? HeadShotDamage : Damage;

				UGameplayStatics::ApplyDamage(
					BaseCharacter,
					DamageToCause,
					InstigatorController,
					this,
					UDamageType::StaticClass()
				);
			}
			/*if (!HasAuthority() && bUseServerSideRewind)
			{
				BlasterOwnerCharacter = BlasterOwnerCharacter == nullptr ? Cast<ABaseCharacter>(OwnerPawn) : BlasterOwnerCharacter;
				BlasterOwnerController = BlasterOwnerController == nullptr ? Cast<AInGamePlayerController>(InstigatorController) : BlasterOwnerController;
				if (BlasterOwnerController && BlasterOwnerCharacter && BlasterOwnerCharacter->GetLagCompensation() && BlasterOwnerCharacter->IsLocallyControlled())
				{
					BlasterOwnerCharacter->GetLagCompensation()->ServerScoreRequest(
						BaseCharacter,
						Start,
						HitTarget,
						BlasterOwnerController->GetServerTime() - BlasterOwnerController->SingleTripTime
					);
				}
			}*/
		}
		if (ImpactParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				ImpactParticles,
				FireHit.ImpactPoint,
				FireHit.ImpactNormal.Rotation()
			);
		}
		if (HitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				HitSound,
				FireHit.ImpactPoint
			);
		}

		if (MuzzleFlash)
		{
			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				MuzzleFlash,
				SocketTransform
			);
		}
		if (FireSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				FireSound,
				GetActorLocation()
			);
		}
	}
}

void AHitScanWeapon::WeaponTraceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutHit)
{
	UWorld* World = GetWorld();
	if (World)
	{
		FVector End = TraceStart + (HitTarget - TraceStart) * 1.25f;

		World->LineTraceSingleByChannel(
			OutHit,
			TraceStart,
			End,
			ECollisionChannel::ECC_Visibility
		);
		FVector BeamEnd = End;
		if (OutHit.bBlockingHit)
		{
			BeamEnd = OutHit.ImpactPoint;
		}
		else
		{
			OutHit.ImpactPoint = End;
		}

		if (BeamParticles)
		{
			UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(
				World,
				BeamParticles,
				TraceStart,
				FRotator::ZeroRotator,
				true
			);
			if (Beam)
			{
				Beam->SetVectorParameter(FName("Target"), BeamEnd);
			}
		}
	}
}

void AHitScanWeapon::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	const FString WeaponTablePath(TEXT("DataTable'/Game/Developers/JJS/Weapons/WeaponsDataTable/HitScanWeaponsDataTable.HitScanWeaponsDataTable'"));
	UDataTable* WeaponTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *WeaponTablePath));

	if (WeaponTableObject)
	{
		FHitScanWeaponDataTable* HitScanWeaponDataRow = nullptr;
		
		HitScanWeaponDataRow = WeaponTableObject->FindRow<FHitScanWeaponDataTable>(WeaponName, TEXT(""));

		if (HitScanWeaponDataRow)
		{
			MuzzleFlash = HitScanWeaponDataRow->MuzzleFlash;
			FireSound = HitScanWeaponDataRow->FireSound;
			BeamParticles = HitScanWeaponDataRow->BeamParticles;
			ImpactParticles = HitScanWeaponDataRow->ImpactParticles;
			HitSound = HitScanWeaponDataRow->HitSound;

			//MuzzleSoketName = HitScanWeaponDataRow->MuzzleSoketName;
			//FireType = HitScanWeaponDataRow->FireType;
			//SubHandSoketName = HitScanWeaponDataRow->SubHandSoketName;
		}
	}
}

void AHitScanWeapon::BeginPlay()
{
	Super::BeginPlay();
	//if (BoneToHide != FName(""))
	//{
	//	GetItemMesh()->HideBoneByName(BoneToHide, EPhysBodyOp::PBO_None);
	//}
}