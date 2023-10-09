#include "Character/LtBelica/EAbliltyActionComponent.h"
#include "Global.h"
#include "Character/CBaseCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "particles/PxParticleSystem.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Sound/SoundCue.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Character/LtBelica/ManaBombHologram.h"
#include "Character/LtBelica/ManaBombObject.h"
#include "GameFramework/Character.h"
#include "BaseSystem/ObjectPoolFactory.h"
#include "BaseSystem/InGameModeBase.h"
#include "BaseSystem/PoolObjectActorComponent.h"

UEAbliltyActionComponent::UEAbliltyActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true; 
	CHelpers::GetAsset<UAnimMontage>(&EAbliltyMontage, "AnimMontage'/Game/Developers/JJS/LtBelica/Animation/E_Ability_Montage.E_Ability_Montage'");

	CHelpers::GetClass<AManaBombHologram>(&ManaBombHologramClass, "Blueprint'/Game/Developers/JJS/LtBelica/Ablility/BP_ManaBombHolo.BP_ManaBombHolo_C'");

	CHelpers::GetClass<AManaBombObject>(&ManaBombClass, "Blueprint'/Game/Developers/JJS/LtBelica/Ablility/BP_ManaBomb.BP_ManaBomb_C'");

}

void UEAbliltyActionComponent::BeginPlay()
{
	ManaBombHologramActor = GetWorld()->SpawnActor<AManaBombHologram>(ManaBombHologramClass, FVector::ZeroVector, FRotator::ZeroRotator);
	ManaBombHologramActor->SetHologramScale(FVector(3));
	IsCoolTiming = false;
	IntervalCoolTime = 0.1f;
	CurCoolTime = 0;
	MaxCoolTime = 5.0f;
	ObjectPoolFactory = CHelpers::GetComponent<UObjectPoolFactory>(GetWorld()->GetAuthGameMode());
	if (ObjectPoolFactory != nullptr)
	{
		ObjectPoolFactory->CreateObject(5, ManaBombClass);
	}
}

void UEAbliltyActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	CheckFalse(IsAbiliting);
	
	FVector start, end, direction;
	OwnerCharacter->GetLocationAndDirection(start, end, direction,false,0.0f,0.0f);
	ManaBombHologramActor->SetHologramLocation(end);
}

void UEAbliltyActionComponent::SetOwnerCharacter(ACharacter* character)
{
	OwnerCharacter = Cast<ACBaseCharacter>(character);

}


void UEAbliltyActionComponent::OnStartAction()
{
	if (IsCoolTiming)
		return;
	if (!IsAbiliting)
	{
		IsAbiliting = true;
		OwnerCharacter->bUseControllerRotationYaw = true;
		OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
		SetComponentTickEnabled(true);
		ManaBombHologramActor->SetActive(true);
	}
	else
	{
		IsAbiliting = false;
		OwnerCharacter->bUseControllerRotationYaw = false;
		OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
		SetComponentTickEnabled(false);
		ManaBombHologramActor->SetActive(false);
	}
}

void UEAbliltyActionComponent::HologramAction()
{
	IsAbiliting = false;
	SetComponentTickEnabled(false);
	ManaBombHologramActor->SetActive(false);

	IsCoolTiming = true;
	GetWorld()->GetTimerManager().SetTimer(CoolTimeHandle, this, &UEAbliltyActionComponent::CoolTimeUpdate, IntervalCoolTime, true);

	OwnerCharacter->PlayAnimMontage(EAbliltyMontage);
}

void UEAbliltyActionComponent::BeginNotifyAction()
{
}

void UEAbliltyActionComponent::MiddleNotifyAction()
{
	AManaBombObject* manaBomb;
	manaBomb = Cast<AManaBombObject>(ObjectPoolFactory->SpawnObject(ManaBombClass));

	manaBomb->SetBombLocation(ManaBombHologramActor->GetActorLocation());
	manaBomb->SetBombScale(ManaBombHologramActor->GetActorScale().X * 50.0f);
	manaBomb->PoolObject->SetActorLifeTime(3.0f);
	manaBomb->PoolObject->SetActive(true);
}

void UEAbliltyActionComponent::EndNotifyAction()
{
}
