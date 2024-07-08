#include "Character/Animation/CharacterAnimInstance.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/BaseCharacter.h"
#include "WorldObjects/Item/Weapon.h"
#include "WorldObjects/Item/WeaponType.h"
#include "Character/CombatState.h"
#include "Net/UnrealNetwork.h"
#include "Character/Components/CombatComponent.h"
#include "Engine/SkeletalMeshSocket.h"

UCharacterAnimInstance::UCharacterAnimInstance():
	Speed(0.f),
	bIsInAir(false),
	bIsAccelerating(false),
	MovementOffsetYaw(0.f),
	LastMovementOffsetYaw(0.f),
	bAiming(false),
	CharacterRotator(FRotator(0.f)),
	CharacterRotatorLastFrame(FRotator(0.f)),
	//TIPCharacterYaw(0.f),
	//TIPCharacterYawLastFrame(0.f),
	YawDelta(0.f),
	RootYawOffset(0.f),
	Pitch(0.f),
	bReloading(false),
	OffsetState(EOffsetState::EOS_Hip),
	RecoilWeight(1.0f),
	//bTurningInPlace(false),
	EquippedWeaponType(EWeaponType::EWT_DefaultMAX),
	bShouldUseFABRIK(false)
{

}

void UCharacterAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	
}

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	OwnerCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());
}

void UCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	//OwnerCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());

	//bIsInAir = false;

}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	CheckNull(OwnerCharacter);
	if (OwnerCharacter == nullptr)
	{
		OwnerCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());
	}
	if (OwnerCharacter == nullptr) return;
	//FString str = FString::Printf(TEXT("\n"));
	//CharacterInfoStr = FString::Printf(TEXT("\n"));
	if (OwnerCharacter)
	{
		bCrouching = OwnerCharacter->bIsCrouched;
		bReloading = OwnerCharacter->GetCombatState() == ECombatState::ECS_Reloading;
		bEquipping = OwnerCharacter->GetCombatState() == ECombatState::ECS_Equipping;
		bShouldUseFABRIK = OwnerCharacter->GetCombatState() == ECombatState::ECS_Unoccupied || OwnerCharacter->GetCombatState() == ECombatState::ECS_FireTimerInProgress;

		FVector Velocity{ OwnerCharacter->GetVelocity() };
		Velocity.Z = 0;
		Speed = Velocity.Size();

		bIsInAir = OwnerCharacter->GetCharacterMovement()->IsFalling();
		bIsAccelerating = OwnerCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true: false;
		bAiming = OwnerCharacter->IsAiming();
		//CharacterInfoStr.Append("bAiming:");
		//CharacterInfoStr.Append(bAiming ? "true" : "false");
		//CharacterInfoStr.Append("\n");
		TurningInPlace = OwnerCharacter->GetTurningInPlace();
		bRotateRootBone = OwnerCharacter->GetRotateRootBone();
		//CharacterInfoStr.Append("bRotateRootBone:");
		//CharacterInfoStr.Append(bRotateRootBone?"true" :"false");
		//CharacterInfoStr.Append("\n");
		bElimmed = OwnerCharacter->IsElimmed();
		//bHoldingTheFlag = BlasterCharacter->IsHoldingTheFlag();

		FRotator AimRotation = OwnerCharacter->GetBaseAimRotation();
		FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(OwnerCharacter->GetVelocity());
		FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
		FRotator DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaSeconds, 6.f);
		MovementOffsetYaw = DeltaRotation.Yaw;

		Lean(DeltaSeconds);

		AO_Yaw = OwnerCharacter->GetAO_Yaw();
		AO_Pitch = OwnerCharacter->GetAO_Pitch();
	
		//FString RotationMessage = FString::Printf(TEXT("Base Aim Rotation: %f"), AimRotation.Yaw);
		//	FString MovementRotationMessage = FString::Printf(TEXT("Movement Rotation: %f"), MovementRotation.Yaw);
		//	FString OffsetMessage = FString::Printf(TEXT("Movement Offset Yaw: %f"), MovementOffsetYaw);
		//	UE_LOG(LogTemp, Log, TEXT("pitch :: %f"), OwnerCharacter->GetVelocity().X);
		//	UE_LOG(LogTemp, Log, TEXT("yaw :: %f"), OwnerCharacter->GetVelocity().Y);
		//	UE_LOG(LogTemp, Log, TEXT("roll :: %f"), OwnerCharacter->GetVelocity().Z);
		//	if (GEngine)
		//	{
		//		GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::White, RotationMessage);
		//		GEngine->AddOnScreenDebugMessage(2, 0.f, FColor::White, MovementRotationMessage);
		//		GEngine->AddOnScreenDebugMessage(3, 0.f, FColor::White, OffsetMessage);
		//	}
		//

		if (bReloading)
		{
			OffsetState = EOffsetState::EOS_Reloading;
		}
		else if (bIsInAir)
		{
			OffsetState = EOffsetState::EOS_InAir;
		}
		else if (bAiming)
		{
			OffsetState = EOffsetState::EOS_Aiming;
		}
		else
		{
			OffsetState = EOffsetState::EOS_Hip;
		}
		bEquipped = OwnerCharacter->GetCombatComp()->GetEquippedWeapon()==nullptr?false:true;
		if (OwnerCharacter->GetCombatComp()->GetEquippedWeapon() && OwnerCharacter->GetMesh())
		{
			LeftHandTransform = OwnerCharacter->GetCombatComp()->GetEquippedWeapon()->GetItemMesh()->GetSocketTransform(OwnerCharacter->GetCombatComp()->GetEquippedWeapon()->GetSubHandSoketName(), ERelativeTransformSpace::RTS_World);
			FVector OutPosition;
			FRotator OutRotation;
			OwnerCharacter->GetMesh()->TransformToBoneSpace(FName("hand_r"), LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);
			LeftHandTransform.SetLocation(OutPosition);
			LeftHandTransform.SetRotation(FQuat(OutRotation));

			if (OwnerCharacter->IsLocallyControlled())
			{
				bLocallyControlled = true;
				FTransform MuzzleTransform = OwnerCharacter->GetCombatComp()->GetEquippedWeapon()->GetItemMesh()->GetSocketTransform(OwnerCharacter->GetCombatComp()->GetEquippedWeapon()->GetMuzzleSoketName(), ERelativeTransformSpace::RTS_World);
				FTransform WeaponTransform = OwnerCharacter->GetCombatComp()->GetEquippedWeapon()->GetItemMesh()->GetSocketTransform(FName("WeaponDirection"), ERelativeTransformSpace::RTS_World);
				RightHandTransform = OwnerCharacter->GetCombatComp()->GetEquippedWeapon()->GetItemMesh()->GetSocketTransform(FName("WeaponMainHandSocket"), ERelativeTransformSpace::RTS_World);

				//(손잡이와 머즐의 직각 부분 - 머즐),  (손잡이 - 머즐) 두 방향벡터의 회전값
				FRotator WeaponDeltaRotation = UKismetMathLibrary::FindLookAtRotation(MuzzleTransform.GetLocation(), WeaponTransform.GetLocation()) - UKismetMathLibrary::FindLookAtRotation(MuzzleTransform.GetLocation(), RightHandTransform.GetLocation());

				//DrawDebugPoint(GetWorld(), RightHandTransform.GetLocation(), 10.f, FColor::Orange, false, 2.f);
				//DrawDebugPoint(GetWorld(), WeaponTransform.GetLocation(), 10.f, FColor::Red, false, 2.f);

				//FRotator WeaponMuzzleDeltaRotation = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetCombatComp()->GetHitTarget(), WeaponTransform.GetLocation()) - UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetCombatComp()->GetHitTarget(),MuzzleTransform.GetLocation());


				FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(RightHandTransform.GetLocation(), RightHandTransform.GetLocation() + (RightHandTransform.GetLocation() - OwnerCharacter->GetCombatComp()->GetHitTarget()));
				LookAtRotation -= WeaponDeltaRotation;
				//LookAtRotation += WeaponMuzzleDeltaRotation;
				RightHandRotation = FMath::RInterpTo(RightHandRotation, LookAtRotation, DeltaSeconds, 30.f);
				//RightHandRotation = WeaponLookAtRotation;//FMath::RInterpTo(RightHandRotation, WeaponLookAtRotation, DeltaSeconds, 30.f);
				//DrawDebugLine(GetWorld(), RightHandTransform.GetLocation(), RightHandTransform.GetLocation() + RightHandRotation.Vector() * -2000, FColor::Green, false, 2.f);
				//CharacterInfoStr.Append("WeaponLookAtRotation:");
				//CharacterInfoStr.Append(WeaponLookAtRotation.ToString());
				//CharacterInfoStr.Append("\n");
				//CharacterInfoStr.Append("RightHandRotation:");
				//CharacterInfoStr.Append(RightHandRotation.ToString());
				//CharacterInfoStr.Append("\n");
				
				//RightHandRotation.Pitch -= 13;//RightHandRotation.Pitch + (RightHandRotation.Pitch - WeaponLookAtRotation.Pitch)*3;
				//DrawDebugLine(GetWorld(), RightHandTransform.GetLocation(), RightHandTransform.GetLocation() + RightHandRotation.Vector()*-2000, FColor::Green, false, 2.f);
				//DrawDebugLine(GetWorld(), WeaponTransform.GetLocation(), WeaponTransform.GetLocation() + WeaponLookAtRotation.Vector() * -2000, FColor::Red, false, 2.f);
			}
		}
		//CharacterInfoStr.Append("IsLocallyControlled:");
		//CharacterInfoStr.Append(OwnerCharacter->IsLocallyControlled() ? "true" : "false");
		//CharacterInfoStr.Append("\n");
		//CharacterInfoStr.Append("GetCombatState:");
		//CharacterInfoStr.Append(UEnum::GetValueAsString(OwnerCharacter->GetCombatState()));
		//CharacterInfoStr.Append("\n");
		//CharacterInfoStr.Append("bFinishedSwapping:");
		//CharacterInfoStr.Append(OwnerCharacter->bFinishedSwapping ? "true" : "false");
		//CharacterInfoStr.Append("\n");
		bool bFABRIKOverride = OwnerCharacter->IsLocallyControlled() &&
			OwnerCharacter->GetCombatState() != ECombatState::ECS_ThrowingGrenade &&
			OwnerCharacter->bFinishedSwapping;
		if (bFABRIKOverride)
		{
			//CharacterInfoStr.Append("bFABRIKOverride:");
			//CharacterInfoStr.Append(bFABRIKOverride ? "true" : "false");
			//CharacterInfoStr.Append("\n");
			bShouldUseFABRIK = !OwnerCharacter->IsLocallyReloading();
		}
		bUseAimOffsets = OwnerCharacter->GetCombatState() == ECombatState::ECS_Unoccupied;//&& !OwnerCharacter->GetDisableGameplay();
		bTransformRightHand = OwnerCharacter->GetCombatState() == ECombatState::ECS_Unoccupied;// && !BlasterCharacter->GetDisableGameplay();
	}
	//CharacterInfoStr.Append("bShouldUseFABRIK:");
	//CharacterInfoStr.Append(bShouldUseFABRIK ? "true" : "false");
	//CharacterInfoStr.Append("\n");
	//CalculateAO_Pitch();
	//TurnInPlace();
}

void UCharacterAnimInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UCharacterAnimInstance::TurnInPlace()
{
	if (OwnerCharacter == nullptr) return;

	//if (Speed > 0 || bIsInAir)
	//{
	//	RootYawOffset = 0.f;
	//	TIPCharacterYaw = OwnerCharacter->GetActorRotation().Yaw;
	//	TIPCharacterYawLastFrame = TIPCharacterYaw;
	//	RotationCurveLastFrame = 0.f;
	//	RotationCurve = 0.f;
	//}
	//else
	//{
	//	TIPCharacterYawLastFrame = TIPCharacterYaw;
	//	TIPCharacterYaw = OwnerCharacter->GetActorRotation().Yaw;
	//	const float TIPYawDelta{ TIPCharacterYaw - TIPCharacterYawLastFrame };
	//	CharacterInfoStr.Append("OwnerCharacter->GetActorRotation().Yaw:");
	//	CharacterInfoStr.Append(FString::SanitizeFloat(OwnerCharacter->GetActorRotation().Yaw));
	//	CharacterInfoStr.Append("\n");
	//	//RootYawOffset -= TIPYawDelta;
	//	//if (GEngine)
	//	//{
	//	//	GEngine->AddOnScreenDebugMessage(1, -1, FColor::White, FString::Printf(TEXT("TIPCharacterYaw: %f"), TIPCharacterYaw));
	//	//	GEngine->AddOnScreenDebugMessage(2, -1, FColor::White, FString::Printf(TEXT("RootYawOffset: %f"), RootYawOffset));
	//	//}

	//	RootYawOffset = UKismetMathLibrary::NormalizeAxis(RootYawOffset - TIPYawDelta);

	//	const float Turning{ GetCurveValue(TEXT("Turning")) };

	//	if (Turning > 0)
	//	{
	//		bTurningInPlace = true;
	//		RotationCurveLastFrame = RotationCurve;
	//		RotationCurve = GetCurveValue(TEXT("Rotation"));

	//		const float DeltaRotation{ RotationCurve - RotationCurveLastFrame };

	//		RootYawOffset > 0 ? RootYawOffset -= DeltaRotation : RootYawOffset += DeltaRotation;

	//		const float ABSRootYawOffset{ FMath::Abs(RootYawOffset) };

	//		if (ABSRootYawOffset > 90.f)
	//		{
	//			const float YawExcess{ ABSRootYawOffset - 90.f };
	//			RootYawOffset > 0 ? RootYawOffset -= YawExcess : RootYawOffset += YawExcess;
	//		}
	//	}
	//	else
	//	{
	//		bTurningInPlace = false;
	//	}
	//}
	//// set recoil
	//if (bTurningInPlace)
	//{
	//	if (bReloading || bEquipping)
	//	{
	//		RecoilWeight = 1.f;
	//	}
	//	else
	//	{
	//		RecoilWeight = 0.f;
	//	}
	//}
	//else
	//{
	//	if (bCrouching)
	//	{
	//		if (bReloading)
	//		{
	//			RecoilWeight = 1.f;
	//		}
	//		else
	//		{
	//			RecoilWeight = 0.1f;
	//		}
	//	}
	//	else
	//	{
	//		if (bAiming || bReloading || bEquipping)
	//		{
	//			RecoilWeight = 1.f;
	//		}
	//		else
	//		{
	//			RecoilWeight = 0.5f;
	//		}
	//	}
	//}
	//CalculateAO_Pitch();
}

void UCharacterAnimInstance::Lean(float DeltaTime)
{
	if (OwnerCharacter == nullptr) return;

	CharacterRotatorLastFrame = CharacterRotator;
	CharacterRotator = OwnerCharacter->GetActorRotation();

	const FRotator Delta{ UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotator,CharacterRotatorLastFrame) };

	const double Target{ (Delta.Yaw) / DeltaTime };
	const double Interp{ FMath::FInterpTo(YawDelta, Target, DeltaTime,6.f) };
	YawDelta = FMath::Clamp(Interp, -90.f, 90.f);
}

void UCharacterAnimInstance::CalculateAO_Pitch()
{
	//Pitch = OwnerCharacter->GetBaseAimRotation().Pitch;
	AO_Pitch = OwnerCharacter->GetBaseAimRotation().Pitch;
	if (Pitch > 90.f && !OwnerCharacter->IsLocallyControlled())
	{
		// map pitch from [270, 360) to [-90, 0)
		FVector2D InRange(270.f, 360.f);
		FVector2D OutRange(-90.f, 0.f);
		//Pitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AO_Pitch);
		AO_Pitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AO_Pitch);
	}
}