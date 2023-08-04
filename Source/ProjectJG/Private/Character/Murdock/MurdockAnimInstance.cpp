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

	//IsFiring = Murdock->GetLtBelicaWeaponIsFiring();
}
