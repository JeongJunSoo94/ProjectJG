#include "Character/Enemies/MeleeEnemy/MeleeAttackActionComponent.h"
#include "Global.h"
#include "Animation/AnimMontage.h"
#include "Character/Enemies/MeleeEnemy/MeleeEnemyCharacter.h"
#include "GameFramework/Character.h"

UMeleeAttackActionComponent::UMeleeAttackActionComponent()
{
}

void UMeleeAttackActionComponent::SetOwnerCharacter(ACharacter* character)
{
	OwnerCharacter = Cast<AMeleeEnemyCharacter>(character);
}

void UMeleeAttackActionComponent::OnAction()
{
}

void UMeleeAttackActionComponent::BeginAction()
{
	OwnerCharacter->isFullBody = true;
}

void UMeleeAttackActionComponent::EndAction()
{
	OwnerCharacter->isAttacked = false;
}
