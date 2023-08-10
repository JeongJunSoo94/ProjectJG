
#include "Character/Murdock/MurdockShieldSkillComponent.h"
#include "Global.h"
#include "Character/CBaseCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Engine/StaticMeshActor.h"
#include "particles/PxParticleSystem.h"
#include "Particles/ParticleSystem.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
UMurdockShieldSkillComponent::UMurdockShieldSkillComponent()
{
	CHelpers::GetAsset<UAnimMontage>(&ShieldAnim, "AnimMontage'/Game/Developers/GohyeongJu/Characters/Murdock/Montages/Shield_Montage.Shield_Montage'");
	
	CHelpers::GetAsset<UParticleSystem>(&Shield, "ParticleSystem'/Game/ParagonMurdock/FX/Particles/Abilities/GunShield/fx/P_GunShield.P_GunShield'");
	CHelpers::GetAsset<UParticleSystem>(&ShieldBreak, "ParticleSystem'/Game/ParagonMurdock/FX/Particles/Abilities/GunShield/fx/P_GunShield_Deactivate.P_GunShield_Deactivate'");


}



void UMurdockShieldSkillComponent::BeginPlay()
{
	Super::BeginPlay();
	Ownercharacter = Cast<ACBaseCharacter>(GetOwner());

	ShieldParticle = UGameplayStatics::SpawnEmitterAttached(Shield, Ownercharacter->GetMesh(), "Shield", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true, EPSCPoolMethod::AutoRelease);
	ShieldParticle->Deactivate();



}

void UMurdockShieldSkillComponent::BeginShield()
{
	Ownercharacter->PlayAnimMontage(ShieldAnim);
	UGameplayStatics::SpawnEmitterAttached(Shield, Ownercharacter->GetMesh(), "Shield", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true, EPSCPoolMethod::AutoRelease);
	
	Ownercharacter->bUseControllerRotationYaw = true;
	Ownercharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
}
void UMurdockShieldSkillComponent::BreakShield()
{ 
	Ownercharacter->PlayAnimMontage(ShieldAnim,1.0f,"EndLoop");
	ShieldParticle->Deactivate();
	UGameplayStatics::SpawnEmitterAttached(ShieldBreak, Ownercharacter->GetMesh(), "Shield", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true, EPSCPoolMethod::AutoRelease);

	Ownercharacter->bUseControllerRotationYaw = false;
	Ownercharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
}

void UMurdockShieldSkillComponent::LoopShieldMontage()
{
	Clog::Log("BeginLoop");
	Ownercharacter->PlayAnimMontage(ShieldAnim, 1.0f, "BeginLoop");
}