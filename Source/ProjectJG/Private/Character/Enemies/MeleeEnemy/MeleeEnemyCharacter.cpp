#include "Character/Enemies/MeleeEnemy/MeleeEnemyCharacter.h"
#include "Global.h"
#include "Character/Enemies/MeleeEnemy/MeleeEnemyAIController.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Character/Interface/Damageable.h"
#include "Character/Enemies/MeleeEnemy/MeleeAttackActionComponent.h"
#include "Animation/AnimMontage.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Components/StatusComponent.h"

void AMeleeEnemyCharacter::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckNull(OverlappedComponent);
	CheckNull(OtherActor);
	CheckNull(OtherComp);
	IDamageable* character = Cast<IDamageable>(OtherActor);
	CheckNull(character);
	character->TakeDamage(10.0f);
}

AMeleeEnemyCharacter::AMeleeEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/ParagonMinions/Characters/Minions/White_Camp_Minion/Meshes/Minion_melee.Minion_melee'");
	GetMesh()->SetSkeletalMesh(mesh);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	
	CHelpers::CreateComponent<UBoxComponent>(this, &WeaponCollisionBox, "WeaponCollisionBox",GetCapsuleComponent());
	WeaponCollisionBox->bHiddenInGame = false;
	
	CHelpers::GetClass<AMeleeEnemyAIController>(&aicontroller, "Blueprint'/Game/Developers/JJS/Enemy/BP_MeleeEnemyAIController.BP_MeleeEnemyAIController_C'");
	AIControllerClass = aicontroller;

	CHelpers::CreateActorComponent<UMeleeAttackActionComponent>(this, &MeleeActionComponent, "MeleeActionComponent");
	MeleeActionComponent->SetOwnerCharacter(this);

	CHelpers::GetAsset<UAnimMontage>(&DeathMontage, "AnimMontage'/Game/Developers/JJS/Enemy/Montage/MeleeDeath_Montage.MeleeDeath_Montage'");

}

void AMeleeEnemyCharacter::BeginPlay()
{
	SetHealthWidgetSizeAndLocation(FVector(0, 0, 150), FVector2D(120, 20));
	Super::BeginPlay();
	WeaponCollisionBox->SetBoxExtent(FVector(1.0f, 30.0f, 10.0f));
	WeaponCollisionBox->AttachToComponent(this->GetMesh(),
			FAttachmentTransformRules(EAttachmentRule::KeepRelative, true),
		WeaponCollisionSocket);
	WeaponCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AMeleeEnemyCharacter::OnComponentBeginOverlap);
	isFullBody = false;
	isAttacked = false;
	CollisionStateNotifyEnd();
}

void AMeleeEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMeleeEnemyCharacter::MeleeAttack()
{
	if (!isAttacked)
	{
		isAttacked = true;
		eCharacterStateFlags = ECharacterStateFlags::ATTACKING;
		MeleeActionComponent->OnStartAction();
	}
}

UCActionComponent* AMeleeEnemyCharacter::GetActionComponent()
{
	return MeleeActionComponent;
}

void AMeleeEnemyCharacter::Die()
{
	eCharacterStateFlags = ECharacterStateFlags::DEAD;
	isFullBody = true;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PlayAnimMontage(DeathMontage);
}

void AMeleeEnemyCharacter::BeginNotifyAction()
{
	switch (eCharacterStateFlags)
	{
		case ECharacterStateFlags::ATTACKING:
		{
			UCActionComponent* actionComp = GetActionComponent();
			CheckNull(actionComp);
			actionComp->BeginNotifyAction();
		}
		break;
		case ECharacterStateFlags::DEAD:
		{
			isFullBody = true;
			int num = FMath::RandRange(0, 2);
			switch (num)
			{
			case 0:
			{
				//GetMesh()->GetAnimInstance()->Montage_JumpToSection("Fwd_2", DeathMontage);
			}break;
			case 1:
			{
				GetMesh()->GetAnimInstance()->Montage_JumpToSection("Fwd_2", DeathMontage);
			}break;
			}
		}
		break;
	}
}
void AMeleeEnemyCharacter::MiddleNotifyAction()
{
	switch (eCharacterStateFlags)
	{
	case ECharacterStateFlags::ATTACKING:
	{
		UCActionComponent* actionComp = GetActionComponent();
		CheckNull(actionComp);
		actionComp->MiddleNotifyAction();
	}
	break;
	case ECharacterStateFlags::DEAD:
	{
	}
	break;
	}
}
void AMeleeEnemyCharacter::EndNotifyAction()
{
	switch (eCharacterStateFlags)
	{
	case ECharacterStateFlags::ATTACKING:
	{
		UCActionComponent* actionComp = GetActionComponent();
		CheckNull(actionComp);
		actionComp->EndNotifyAction();
	}
	break;
	case ECharacterStateFlags::DEAD:
	{
		GetMesh()->GetAnimInstance()->Montage_Pause(DeathMontage);//MontagePause(DeathMontage);
		//isFullBody = false;
		//Destroy();
	}
	break;
	}
}

void AMeleeEnemyCharacter::CollisionStateNotifyBegin()
{
	WeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}
void AMeleeEnemyCharacter::CollisionStateNotifyEnd()
{
	WeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMeleeEnemyCharacter::RegistBlackBoardDatas(class UBlackboardComponent* blackboard)
{
	blackboard->SetValueAsObject(TEXT("Status"), Status);
}