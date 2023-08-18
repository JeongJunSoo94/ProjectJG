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
	CHelpers::GetClass<AManaBombHologram>(&ManaBombHologramClass, "Blueprint'/Game/Developers/JJS/LtBelica/Ablility/BP_ManaBombHolo.BP_ManaBombHolo_C'");

	CHelpers::GetClass<AManaBombObject>(&ManaBombClass, "Blueprint'/Game/Developers/JJS/LtBelica/Ablility/BP_ManaBomb.BP_ManaBomb_C'");

}

void UEAbliltyActionComponent::BeginPlay()
{
	ManaBombHologramActor = GetWorld()->SpawnActor<AManaBombHologram>(ManaBombHologramClass, FVector::ZeroVector, FRotator::ZeroRotator);
	ManaBombHologramActor->SetHologramScale(FVector(3));
}

void UEAbliltyActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	CheckFalse(IsAbiliting);
	
	FVector start, end, direction;
	OwnerCharacter->GetLocationAndDirection(start, end, direction,false,0.0f,0.0f);
	ManaBombHologramActor->SetHologramLocation(end);
}

void UEAbliltyActionComponent::SetOwnerCharacter(ACBaseCharacter* character)
{
	OwnerCharacter = character;
	CHelpers::CreateActorComponent<UObjectPoolFactory>(OwnerCharacter, &ObjectPoolFactory, "ManaBombFactory");
	ObjectPoolFactory->PoolSize = 5;
	ObjectPoolFactory->PooledObjectSubclass = ManaBombClass;
	ObjectPoolFactory->Initialized();
}


void UEAbliltyActionComponent::OnAction()
{
	if (!IsAbiliting)
	{
		IsAbiliting = true;
		SetComponentTickEnabled(true);
		ManaBombHologramActor->SetActive(true);
	}
	else
	{
		IsAbiliting = false;
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

	manaBomb->SetActive(true);
}

void UEAbliltyActionComponent::BeginAction()
{
}

void UEAbliltyActionComponent::EndAction()
{
}
