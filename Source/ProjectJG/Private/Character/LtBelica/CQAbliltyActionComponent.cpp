#include "Character/LtBelica/CQAbliltyActionComponent.h"
#include "Global.h"
#include "Character/CBaseCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "particles/PxParticleSystem.h"
#include "Particles/ParticleSystem.h"
#include "Engine/StaticMeshActor.h"
#include "Sound/SoundCue.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceConstant.h"

UCQAbliltyActionComponent::UCQAbliltyActionComponent()
{
	CHelpers::GetAsset<UAnimMontage>(&QAbliltyMontage, "AnimMontage'/Game/Developers/USER/Character/Q_Ability_Montage.Q_Ability_Montage'");
	CHelpers::GetAsset<UParticleSystem>(&ImpactParticle, "ParticleSystem'/Game/ParagonLtBelica/FX/Particles/Belica/Abilities/ManaBomb/FX/P_ManaBombGround.P_ManaBombGround'");
}

void UCQAbliltyActionComponent::BeginPlay()
{
	UParticleSystemComponent* par = UGameplayStatics::SpawnEmitterAttached(ImpactParticle, OwnerCharacter->GetMesh(), "hologramSocket", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true, EPSCPoolMethod::AutoRelease);

}

void UCQAbliltyActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
}

void UCQAbliltyActionComponent::OnAction()
{
	OwnerCharacter->PlayAnimMontage(QAbliltyMontage);
}

void UCQAbliltyActionComponent::BeginAction()
{
	IsAbiliting = true;
}

void UCQAbliltyActionComponent::EndAction()
{
	FTransform transform = OwnerCharacter->GetMesh()->GetSocketTransform("hand_r");
	FVector handLocation = transform.GetLocation();
	FRotator handRotator = transform.GetRotation().Rotator();

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, handLocation, handRotator);
	IsAbiliting = false;
}
