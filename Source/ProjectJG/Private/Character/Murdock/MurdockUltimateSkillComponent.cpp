
#include "Character/Murdock/MurdockUltimateSkillComponent.h"
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

UMurdockUltimateSkillComponent::UMurdockUltimateSkillComponent()
{
	CHelpers::GetAsset<UAnimMontage>(&UltimateAnim, "AnimMontage'/Game/Developers/GohyeongJu/Characters/Murdock/Montages/Ultimate_Montage.Ultimate_Montage'");
	CHelpers::GetAsset<UAnimMontage>(&UltimateLoopAnim, "AnimMontage'/Game/Developers/GohyeongJu/Characters/Murdock/Montages/Ultimate_Loop_Montage.Ultimate_Loop_Montage'");


}

void UMurdockUltimateSkillComponent::BeginUltimate()
{
	Ownercharacter->PlayAnimMontage(UltimateAnim, 1.0f, "Default");

}

void UMurdockUltimateSkillComponent::LoopUltimate()
{
	Ownercharacter->StopAnimMontage(UltimateAnim);
	Ownercharacter->PlayAnimMontage(UltimateLoopAnim);
}

void UMurdockUltimateSkillComponent::EndUltimate()
{
	Ownercharacter->StopAnimMontage(UltimateLoopAnim);
	Ownercharacter->PlayAnimMontage(UltimateAnim,1.0f,"LoopOutUltimate");
}


void UMurdockUltimateSkillComponent::BeginPlay()
{
	Super::BeginPlay();
	Ownercharacter = Cast<ACBaseCharacter>(GetOwner());

}
