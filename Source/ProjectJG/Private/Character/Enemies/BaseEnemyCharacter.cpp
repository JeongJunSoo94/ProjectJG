#include "Character/Enemies/BaseEnemyCharacter.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Character/Components/StatusComponent.h"
#include "Widgets/StatusUserWidget.h"
#include "Widgets/HealthWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Character/Enemies/AIController/BaseAIController.h"
#include "WorldObjects/FXActor/DamageFXActor.h"
#include "BaseSystem/ObjectPoolFactory.h"
#include "BaseSystem/InGameModeBase.h"
#include "BaseSystem/PoolObjectActorComponent.h"

ABaseEnemyCharacter::ABaseEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	CHelpers::CreateActorComponent<UStatusComponent>(this, &Status, "Status");

	CHelpers::CreateComponent<UWidgetComponent>(this, &HealthWidget, "HealthWidget", GetMesh());

	CHelpers::GetClass<ADamageFXActor>(&DamageWidgetClass, "Blueprint'/Game/Developers/JJS/FXActor/BP_DamageActor.BP_DamageActor_C'");

	TSubclassOf<UHealthWidget> healthClass;
	CHelpers::GetClass<UHealthWidget>(&healthClass, "WidgetBlueprint'/Game/Developers/USER/Character/WB_Health.WB_Health_C'");
	HealthWidget->SetWidgetClass(healthClass);
	SetHealthWidgetSizeAndLocation(FVector(0, 0, 200), FVector2D(120, 20));
	HealthWidget->SetWidgetSpace(EWidgetSpace::Screen);

	CHelpers::CreateActorComponent(this, &PoolObject, "PoolObject");
}

void ABaseEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	//HealthWidget->InitWidget();
	//Cast<UHealthWidget>(HealthWidget->GetUserWidgetObject())->Update(Status->GetHealth(), Status->GetMaxHealth());
	BaseAIController = Cast<ABaseAIController>(GetController());
	ObjectPoolFactory = CHelpers::GetComponent<UObjectPoolFactory>(GetWorld()->GetAuthGameMode());
	if (ObjectPoolFactory != nullptr)
	{
		ObjectPoolFactory->SpawnObject(DamageWidgetClass);
	}
}

void ABaseEnemyCharacter::SetHealthWidgetSizeAndLocation(FVector location, FVector2D size)
{
	HealthWidget->SetRelativeLocation(location);
	HealthWidget->SetDrawSize(size);
}

void ABaseEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (DamageValue > 0)
		Damaged(DamageValue);
	

}

void ABaseEnemyCharacter::BeginHitEffect(AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{

}

void ABaseEnemyCharacter::SetImpactVectorFrom(FVector& ProjectileVector)
{
}

void ABaseEnemyCharacter::RegistBlackBoardDatas(UBlackboardComponent* blackboard)
{
}

float ABaseEnemyCharacter::TakeDamage(float Damage)
{
	DamageValue += Damage;
	if (ObjectPoolFactory == nullptr)
	{
		return DamageValue;
	}
	ADamageFXActor* DamageActor = Cast<ADamageFXActor>(ObjectPoolFactory->SpawnObject(DamageWidgetClass));
	if (DamageActor == nullptr)
	{
		return DamageValue;
	}
	DamageActor->SetDamageText(Damage);

	FTransform Transform = DamageActor->GetTransform();
	DamageActor->SetActorTransform(GetActorTransform());
	//DamageActor->SetDamageWidgetSizeAndLocation(GetActorLocation(), FVector2D(120, 20));
	DamageActor->PoolObject->SetActorLifeTime(2.0f);

	DamageActor->PoolObject->SetActive(true);

	if(!HealthWidget->GetUserWidgetObject()->GetIsVisible())
		HealthWidget->GetUserWidgetObject()->SetVisibility(ESlateVisibility::HitTestInvisible);

	return DamageValue;

	//DamageInstigator = EventInstigator;
	//DamageValue = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	//State->SetHittedMode();
	//return Status->GetHealth();
}

void ABaseEnemyCharacter::Damaged(float totalAmount)
{
	Status->SubHealth(totalAmount);
	Cast<UHealthWidget>(HealthWidget->GetUserWidgetObject())->Update(Status->GetHealth(), Status->GetMaxHealth());
	if (Status->GetHealth() <= 0)
	{
		Die();
	}
	DamageValue = 0;
}

void ABaseEnemyCharacter::Die()
{
	CheckNull(BaseAIController);
	eCharacterStateFlags = ECharacterStateFlags::DEAD;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	IsFullBody = true;
	IsDie = true;
	BaseAIController->StopAI();
}
void ABaseEnemyCharacter::Init()
{
	Status->AddHealth(100);
	Cast<UHealthWidget>(HealthWidget->GetUserWidgetObject())->Update(Status->GetHealth(), Status->GetMaxHealth());
	HealthWidget->GetUserWidgetObject()->SetVisibility(ESlateVisibility::Collapsed);
	IsFullBody = false;
	IsDie = false;
	CheckNull(BaseAIController);
	BaseAIController->StartAI();
	StopAnimMontage();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ABaseEnemyCharacter::ReturnPool()
{
	OnReturnSpawner.Execute(this);
	CheckNull(PoolObject);
	PoolObject->OnReturnToPool.Execute(this);
}