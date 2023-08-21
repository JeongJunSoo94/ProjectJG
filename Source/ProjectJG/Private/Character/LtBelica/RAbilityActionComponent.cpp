#include "Character/LtBelica/RAbilityActionComponent.h"
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
#include "BaseSystem/ObjectPoolFactory.h"
#include "Character/LtBelica/EruptionHologram.h"
#include "Character/LtBelica/EruptionObject.h"

URAbilityActionComponent::URAbilityActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	//CHelpers::GetAsset<UAnimMontage>(&RAbliltyMontage, "AnimMontage'/Game/Developers/JJS/LtBelica/Animation/R_Ability_Montage.R_Ability_Montage'");
}

void URAbilityActionComponent::BeginPlay()
{
}

void URAbilityActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	CheckFalse(IsAbiliting);
}

void URAbilityActionComponent::SetOwnerCharacter(ACBaseCharacter* character)
{
}

void URAbilityActionComponent::OnAction()
{
}

void URAbilityActionComponent::HologramAction()
{
}

void URAbilityActionComponent::BeginAction()
{
}

void URAbilityActionComponent::EndAction()
{
}
