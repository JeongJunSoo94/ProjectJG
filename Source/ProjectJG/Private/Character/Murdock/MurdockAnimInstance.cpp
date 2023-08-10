// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Murdock/MurdockAnimInstance.h"
#include "Global.h"
#include "Character/Murdock/Murdock.h"

void UMurdockAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	Murdock = Cast<AMurdock>(TryGetPawnOwner());
}

void UMurdockAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	CheckNull(Murdock);

	behaviorState = Murdock->GetBehaviorState();

	if (behaviorState == MurdockBehaviorState::EUltimate)
	{
		LookAtYaw = Murdock->GetLookYaw();
		//Clog::Log(LookAtYaw);
			//(OwnerCharacter->GetBaseAimRotation().Yaw - Murdock->GetFrontYaw())*0.2f;
	}

	//IsFiring = Murdock->GetLtBelicaWeaponIsFiring();
}
