#include "Character/LtBelica/CQAbliltyActionComponent.h"
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

UCQAbliltyActionComponent::UCQAbliltyActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	CHelpers::GetAsset<UAnimMontage>(&QAbliltyMontage, "AnimMontage'/Game/Developers/USER/Character/Q_Ability_Montage.Q_Ability_Montage'");
	CHelpers::GetAsset<UParticleSystem>(&GroundPunchParticle, "ParticleSystem'/Game/ParagonLtBelica/FX/Particles/Belica/Abilities/EruptionBeam/FX/P_GroundPunch.P_GroundPunch'");
	CHelpers::GetAsset<UParticleSystem>(&EruptionBeamParticle, "ParticleSystem'/Game/ParagonLtBelica/FX/Particles/Belica/Abilities/EruptionBeam/FX/P_EruptionBeam2.P_EruptionBeam2'");
	CHelpers::GetAsset<UParticleSystem>(&EruptionRocksParticle, "ParticleSystem'/Game/ParagonLtBelica/FX/Particles/Belica/Abilities/EruptionBeam/FX/P_EruptionRocks.P_EruptionRocks'");
	CHelpers::GetAsset<UParticleSystem>(&HandParticle, "ParticleSystem'/Game/ParagonLtBelica/FX/Particles/Belica/Abilities/EruptionBeam/FX/P_QSwing.P_QSwing'");
	CHelpers::GetAsset<UParticleSystem>(&HologramParticle, "ParticleSystem'/Game/ParagonLtBelica/FX/Particles/Belica/Abilities/EruptionBeam/FX/P_QSwing.P_QSwing'");
	
	CHelpers::GetClass<AEruptionHologram>(&EruptionHologramClass, "Blueprint'/Game/Developers/JJS/LtBelica/Ablility/BP_EruptionHolo.BP_EruptionHolo_C'");

	CHelpers::GetClass<AEruptionObject>(&EruptionClass, "Blueprint'/Game/Developers/JJS/LtBelica/Ablility/BP_Eruption.BP_Eruption_C'");

}

void UCQAbliltyActionComponent::BeginPlay()
{
	EruptionHologramActor = GetWorld()->SpawnActor<AEruptionHologram>(EruptionHologramClass, FVector::ZeroVector, FRotator::ZeroRotator);
	EruptionHologramActor->SetHologramScale(FVector(2));
	HandParticleComponent = UGameplayStatics::SpawnEmitterAttached(HandParticle, OwnerCharacter->GetMesh(), "hologramSocket", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true, EPSCPoolMethod::AutoRelease);
	HandParticleComponent->Activate(false);
}

void UCQAbliltyActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	CheckFalse(IsAbiliting);

	FVector start, end, boxScale;
	OwnerCharacter->GetLocationAndDirection(start, end, AbilityDirection, false, 0.0f, 0.0f);
	AbilityDirection = end - OwnerCharacter->GetActorLocation();
	AbilityDirection.Z = OwnerCharacter->GetActorLocation().Z - OwnerCharacter->GetActorScale().Z*100;
	end = OwnerCharacter->GetActorLocation() + AbilityDirection.GetSafeNormal() * EruptionHologramActor->GetHologramBoxScale().X;

	EruptionHologramActor->SetHologramLocation(end);
	EruptionHologramActor->SetHologramRotator(UKismetMathLibrary::Conv_VectorToRotator(AbilityDirection));
}

void UCQAbliltyActionComponent::SetOwnerCharacter(ACharacter* character)
{
	OwnerCharacter = Cast<ACBaseCharacter>(character);
	CHelpers::CreateActorComponent<UObjectPoolFactory>(OwnerCharacter, &ObjectPoolFactory, "EruptionFactory");
	ObjectPoolFactory->PoolSize = 5;
	ObjectPoolFactory->PooledObjectSubclass = EruptionClass;
	ObjectPoolFactory->Initialized();
}

void UCQAbliltyActionComponent::OnStartAction()
{
	if (!IsAbiliting)
	{
		//OwnerCharacter->bUseControllerRotationYaw = true;
		OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
		IsAbiliting = true;
		SetComponentTickEnabled(true);
		EruptionHologramActor->SetActive(true);
	}
	else
	{
		//OwnerCharacter->bUseControllerRotationYaw = false;
		OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
		IsAbiliting = false;
		SetComponentTickEnabled(false);
		EruptionHologramActor->SetActive(false);
	}
}

void UCQAbliltyActionComponent::HologramAction()
{
	IsAbiliting = false;
	SetComponentTickEnabled(false);
	EruptionHologramActor->SetActive(false);
	FTransform transform = OwnerCharacter->GetMesh()->GetSocketTransform("hand_r");
	FVector handLocation = transform.GetLocation();
	FRotator handRotator = transform.GetRotation().Rotator();

	OwnerCharacter->PlayAnimMontage(QAbliltyMontage);
	UGameplayStatics::SpawnEmitterAttached(HandParticle, OwnerCharacter->GetMesh(), "hologramSocket", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true, EPSCPoolMethod::AutoRelease);

}

void UCQAbliltyActionComponent::BeginNotifyAction()
{
	OwnerCharacter->bUseControllerRotationYaw = false;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
}

void UCQAbliltyActionComponent::EndNotifyAction()
{
	FVector box = EruptionHologramActor->GetHologramBoxScale();
	FVector startLocation, intervalLocation, rayStart, rayEnd;
	FHitResult OutHit;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this->GetOwner());

	bool IsHit;

	float intervalX = box.X * 0.25f;

	intervalLocation = EruptionHologramActor->GetActorRotation().Vector() * intervalX;
	startLocation = (EruptionHologramActor->GetActorLocation() + EruptionHologramActor->GetActorLocation().UpVector * EruptionHologramActor->GetHologramBoxScale().Z) - intervalLocation * 4;

	float rayDownInterval = EruptionHologramActor->GetHologramBoxScale().Z * -2.0f;

	AEruptionObject* eruption;
	for (int i = 1; i <= 7; ++i)
	{
		eruption = Cast<AEruptionObject>(ObjectPoolFactory->SpawnObject());
		CollisionParams.AddIgnoredActor(eruption);
		eruption->SetEruptionScale(EruptionHologramActor->GetActorScale().X * 100.0f);
		rayStart = startLocation + intervalLocation * i;
		rayEnd = rayStart + rayStart.UpVector * rayDownInterval;
		IsHit = GetWorld()->LineTraceSingleByChannel(OutHit, rayStart, rayEnd, ECC_Visibility, CollisionParams);
		if (IsHit)
		{
			eruption->SetActorLocation(OutHit.ImpactPoint);
			eruption->SetActorLifeTime(i * 0.1f);
			eruption->SetActive(true);
		}
	}

	FTransform transform = OwnerCharacter->GetMesh()->GetSocketTransform("MuzzleStationary2");
	FVector handLocation = transform.GetLocation();
	FRotator handRotator = transform.Rotator();

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), GroundPunchParticle, handLocation, handRotator + FRotator(0, 0, 180));
	
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EruptionBeamParticle, handLocation, handRotator);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EruptionRocksParticle, handLocation, handRotator + FRotator(45, 0, 0));

	HandParticleComponent->Activate(false);
	OwnerCharacter->bUseControllerRotationYaw = true;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
}
