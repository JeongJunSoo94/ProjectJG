#include "Character/Enemies/MeleeEnemy/MeleeEnemyCharacter.h"
#include "Global.h"
#include "Character/Enemies/MeleeEnemy/MeleeEnemyAIController.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Character/Interface/Damageable.h"
#include "Character/Enemies/MeleeEnemy/MeleeAttackActionComponent.h"
#include "Animation/AnimMontage.h"
#include "Character/Components/StatusComponent.h"
#include "Net/UnrealNetwork.h"
#include "Character/BaseCharacter.h"

void AMeleeEnemyCharacter::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckNull(OverlappedComponent);
	CheckNull(OtherActor);
	CheckNull(OtherComp);
	ABaseCharacter*Player = Cast<ABaseCharacter>(OtherActor);
	if(Player)
		UGameplayStatics::ApplyDamage(OtherActor, Damage, Controller, this, UDamageType::StaticClass());
	//IDamageable* character = Cast<IDamageable>(OtherActor);
	//CheckNull(character);
	//character->TakeDamage(10.0f);
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
	//WeaponCollisionBox->bHiddenInGame = false;

	TSubclassOf<AMeleeEnemyAIController> aicontroller;
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
	if (WeaponCollisionBox != nullptr)
	{
		WeaponCollisionBox->SetBoxExtent(FVector(1.0f, 30.0f, 10.0f));
		WeaponCollisionBox->AttachToComponent(this->GetMesh(),
				FAttachmentTransformRules(EAttachmentRule::KeepRelative, true),
			WeaponCollisionSocket);
	}
	else
	{
		CHelpers::CheckNullComponent<UBoxComponent>(this, &WeaponCollisionBox);
		WeaponCollisionBox->SetBoxExtent(FVector(1.0f, 30.0f, 10.0f));
		WeaponCollisionBox->AttachToComponent(this->GetMesh(),
			FAttachmentTransformRules(EAttachmentRule::KeepRelative, true),
			WeaponCollisionSocket);
	}
	if (HasAuthority())
	{
		WeaponCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AMeleeEnemyCharacter::OnComponentBeginOverlap);
	}
	IsFullBody = false;
	isAttacked = false;
	CollisionStateNotifyEnd();
}

void AMeleeEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//Clog::Log(UEnum::GetValueAsString(eCharacterStateFlags));
	//Clog::Log("isAttacked");
	//Clog::Log(isAttacked);
}

void AMeleeEnemyCharacter::MeleeAttack()
{
	if (!isAttacked)
	{
		if (HasAuthority())
			MulticastAttack();
	}
}

UCActionComponent* AMeleeEnemyCharacter::GetActionComponent()
{
	return MeleeActionComponent;
}

void AMeleeEnemyCharacter::Die()
{
	StopAnimMontage();
	PlayAnimMontage(DeathMontage);
	Super::Die();
}

bool AMeleeEnemyCharacter::CanAttack()
{
	return eCharacterStateFlags!= ECharacterStateFlags::DEAD && !(eCharacterStateFlags == ECharacterStateFlags::ATTACKING);
}

void AMeleeEnemyCharacter::MulticastAttack_Implementation()
{
	//isAttacked = true;
	eCharacterStateFlags = ECharacterStateFlags::ATTACKING;
	MeleeActionComponent->OnStartAction();
}

//void AMeleeEnemyCharacter::Init()
//{
//	Super::Init();
//	isAttacked =false;
//}

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
			IsFullBody = true;
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
		eCharacterStateFlags = ECharacterStateFlags::IDLE;
	}
	break;
	case ECharacterStateFlags::DEAD:
	{
		GetMesh()->GetAnimInstance()->Montage_Pause(DeathMontage);//MontagePause(DeathMontage);
		ReturnPool();
		IsFullBody = false;
		Destroy();
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

//void AMeleeEnemyCharacter::RegistBlackBoardDatas(class UBlackboardComponent* blackboard)
//{
//	//blackboardComp = blackboard;
//}
