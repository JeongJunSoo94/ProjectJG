#include "Character/Enemies/BaseEnemyCharacter.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Character/Components/StatusComponent.h"
#include "Widgets/StatusUserWidget.h"
#include "Widgets/HealthWidget.h"
#include "Components/WidgetComponent.h"
#include "BaseSystem/PoolObjectActorComponent.h"

ABaseEnemyCharacter::ABaseEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	CHelpers::CreateActorComponent<UStatusComponent>(this, &Status, "Status");

	CHelpers::CreateComponent<UWidgetComponent>(this, &HealthWidget, "HealthWidget", GetMesh());

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
	HealthWidget->InitWidget();
	Cast<UHealthWidget>(HealthWidget->GetUserWidgetObject())->Update(Status->GetHealth(), Status->GetMaxHealth());
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
	DamageValue = 0;
}

void ABaseEnemyCharacter::Die()
{
}
void ABaseEnemyCharacter::Init()
{
}

void ABaseEnemyCharacter::ReturnPool()
{
	PoolObject->OnReturnToPool.Execute(this);
}