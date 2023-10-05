// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemies/RangeEnemy/RangeEnemyAnimInstance.h"
#include "Global.h"
#include "Character/Enemies/RangeEnemy/RangeEnemyCharacter.h"

void URangeEnemyAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	RangeEnemyCharacter = Cast<ARangeEnemyCharacter>(TryGetPawnOwner());
}

void URangeEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	CheckNull(RangeEnemyCharacter);
	FullBody = RangeEnemyCharacter->IsFullBody;
	IsDie = RangeEnemyCharacter->IsDie;
}

