#include "Character/Enemies/MeleeEnemy/MeleeAttackActionComponent.h"
#include "Global.h"
#include "Animation/AnimMontage.h"
#include "Character/Enemies/MeleeEnemy/MeleeEnemyCharacter.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"

UMeleeAttackActionComponent::UMeleeAttackActionComponent()
{

	CHelpers::GetAsset<UAnimMontage>(&MeleeMontage, "AnimMontage'/Game/Developers/JJS/Enemy/Montage/MeleeAttack_Montage.MeleeAttack_Montage'");

}

void UMeleeAttackActionComponent::SetOwnerCharacter(ACharacter* character)
{
	OwnerCharacter = Cast<AMeleeEnemyCharacter>(character);
	
}

void UMeleeAttackActionComponent::OnStartAction()
{
	ABPInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
	OwnerCharacter->PlayAnimMontage(MeleeMontage);
}

void UMeleeAttackActionComponent::BeginNotifyAction()
{
	OwnerCharacter->isFullBody = true;
}

void UMeleeAttackActionComponent::MiddleNotifyAction()
{
	ABPInstance->Montage_JumpToSection("Loop", MeleeMontage);
}


void UMeleeAttackActionComponent::EndNotifyAction()
{
	OwnerCharacter->isFullBody = false;
	OwnerCharacter->isAttacked = false;
}
