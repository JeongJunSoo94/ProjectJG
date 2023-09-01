#include "Character/Enemies/MeleeEnemy/MeleeEnemyCharacter.h"
#include "Global.h"
#include "Character/Enemies/MeleeEnemy/MeleeEnemyAIController.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Character/Interface/Damageable.h"

void AMeleeEnemyCharacter::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
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
	
	TSubclassOf<AMeleeEnemyAIController> aicontroller;
	//CHelpers::GetClass<AMeleeEnemyAIController>(&aicontroller, "Blueprint'/Game/Developers/JJS/Enemy/BP_MeleeEnemyAIController.BP_MeleeEnemyAIController_C'");
	//AIControllerClass = aicontroller;

	//CHelpers::GetAsset<UAnimMontage>(&MeleeMontage, "AnimMontage'/Game/Developers/USER/Character/Q_Ability_Montage.Q_Ability_Montage'");

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
}

void AMeleeEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMeleeEnemyCharacter::MeleeAttack()
{

}
