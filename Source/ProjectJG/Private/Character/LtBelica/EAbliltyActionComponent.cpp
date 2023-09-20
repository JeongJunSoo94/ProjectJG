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
	CurCoolTime = 0;
	MaxCoolTime = 5.0f;
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
	CHelpers::CreateActorComponent<UObjectPoolFactory>(OwnerCharacter, &ObjectPoolFactory, "ManaBombFactory");
	ObjectPoolFactory->PoolSize = 5;
	ObjectPoolFactory->PooledObjectSubclass = ManaBombClass;
	ObjectPoolFactory->Initialized();
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
	AManaBombObject* manaBomb;
	manaBomb = Cast<AManaBombObject>(ObjectPoolFactory->SpawnObject());

	manaBomb->SetBombLocation(ManaBombHologramActor->GetActorLocation());
	manaBomb->SetBombScale(ManaBombHologramActor->GetActorScale().X * 50.0f);
	manaBomb->SetActorLifeTime(3.0f);

	IsCoolTiming = true;
	GetWorld()->GetTimerManager().SetTimer(CoolTimeHandle, this, &UEAbliltyActionComponent::CoolTimeUpdate, 0.1f, true);

	OwnerCharacter->PlayAnimMontage(EAbliltyMontage);
	manaBomb->SetActive(true);
}

void UEAbliltyActionComponent::BeginNotifyAction()
{
	Clog::Log("E");
}

void UEAbliltyActionComponent::EndNotifyAction()
{
}

void UEAbliltyActionComponent::CoolTimeUpdate()
{
	CurCoolTime += 0.1f;
	OnUpdateWidgetTimer.Execute(CurCoolTime, MaxCoolTime);
	if (CurCoolTime >= MaxCoolTime)
	{
		IsCoolTiming = false;
		CurCoolTime = 0.0f;
		GetWorld()->GetTimerManager().ClearTimer(CoolTimeHandle);
	}
}