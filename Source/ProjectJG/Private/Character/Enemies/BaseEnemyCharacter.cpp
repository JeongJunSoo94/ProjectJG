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
#include "Net/UnrealNetwork.h"
#include "BaseSystem/BattleGameMode.h"
#include "BaseSystem/InGamePlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Character/Components/ItemDropComponent.h"

ABaseEnemyCharacter::ABaseEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	//CHelpers::CreateActorComponent<UStatusComponent>(this, &Status, "Status");

	CHelpers::CreateComponent<UWidgetComponent>(this, &HealthWidget, "HealthWidget", GetMesh());

	CHelpers::GetClass<ADamageFXActor>(&DamageWidgetClass, "Blueprint'/Game/Developers/JJS/FXActor/BP_DamageActor.BP_DamageActor_C'");

	TSubclassOf<UHealthWidget> healthClass;
	CHelpers::GetClass<UHealthWidget>(&healthClass, "WidgetBlueprint'/Game/Developers/USER/Character/WB_Health.WB_Health_C'");
	HealthWidget->SetWidgetClass(healthClass);
	SetHealthWidgetSizeAndLocation(FVector(0, 0, 200), FVector2D(120, 20));
	HealthWidget->SetWidgetSpace(EWidgetSpace::Screen);

	//CHelpers::CreateActorComponent(this, &PoolObject, "PoolObject");

	ItemDropComp = CreateDefaultSubobject<UItemDropComponent>(TEXT("Combat"));
	ItemDropComp->SetIsReplicated(true);
}

void ABaseEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	HealthWidget->InitWidget();
	Cast<UHealthWidget>(HealthWidget->GetUserWidgetObject())->Update(Health,MaxHealth);
	BaseAIController = Cast<ABaseAIController>(GetController());
	//ObjectPoolFactory = CHelpers::GetComponent<UObjectPoolFactory>(GetWorld()->GetAuthGameMode());
	//if (ObjectPoolFactory != nullptr)
	//{
	//	ObjectPoolFactory->SpawnObject(DamageWidgetClass);
	//}

	if (HasAuthority())
	{
		OnTakeAnyDamage.AddDynamic(this, &ABaseEnemyCharacter::ReceiveDamage);
	}
}

void ABaseEnemyCharacter::SetHealthWidgetSizeAndLocation(FVector location, FVector2D size)
{
	HealthWidget->SetRelativeLocation(location);
	HealthWidget->SetDrawSize(size);
}

void ABaseEnemyCharacter::Destroyed()
{
	Super::Destroyed();
	if (HasAuthority())
	{
		ItemDropComp->SpawnItem();
	}
}

void ABaseEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//void ABaseEnemyCharacter::BeginHitEffect(AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
//{
//
//}
//
//void ABaseEnemyCharacter::SetImpactVectorFrom(FVector& ProjectileVector)
//{
//}

void ABaseEnemyCharacter::RegistBlackBoardDatas(UBlackboardComponent* blackboard)
{
}

void ABaseEnemyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME_CONDITION(ABaseEnemyCharacter, TraceHitItem, COND_OwnerOnly);
	//DOREPLIFETIME(ABaseEnemyCharacter, Inventory);
	DOREPLIFETIME(ABaseEnemyCharacter, Health);
	DOREPLIFETIME(ABaseEnemyCharacter, Shield);
}

void ABaseEnemyCharacter::OnRep_Health(float LastHealth)
{
	//UpdateHUDHealth();
	Cast<UHealthWidget>(HealthWidget->GetUserWidgetObject())->Update(Health, MaxHealth);
	if (Health < LastHealth)
	{
		//PlayHitReactMontage();
	}
}

void ABaseEnemyCharacter::OnRep_Shield(float LastShield)
{
	//UpdateHUDShield();
	if (Shield < LastShield)
	{
		//PlayHitReactMontage();
	}
}

//float ABaseEnemyCharacter::TakeDamage(float Damage)
//{
//	//DamageValue += Damage;
//	////if (ObjectPoolFactory == nullptr)
//	////{
//	////	return DamageValue;
//	////}
//	////ADamageFXActor* DamageActor = Cast<ADamageFXActor>(ObjectPoolFactory->SpawnObject(DamageWidgetClass));
//	////if (DamageActor == nullptr)
//	////{
//	////	return DamageValue;
//	////}
//	//UWorld* const World = GetWorld();
//
//	//ADamageFXActor* DamageActor = Cast<ADamageFXActor>(World->SpawnActor<AActor>(DamageWidgetClass, FVector::ZeroVector, FRotator::ZeroRotator));
//
//	//DamageActor->SetDamageText(Damage);
//
//	//FTransform Transform = DamageActor->GetTransform();
//	//DamageActor->SetActorTransform(GetActorTransform());
//	////DamageActor->SetDamageWidgetSizeAndLocation(GetActorLocation(), FVector2D(120, 20));
//	//DamageActor->PoolObject->SetActorLifeTime(2.0f);
//
//	//DamageActor->PoolObject->SetActive(true);
//
//	//if(!HealthWidget->GetUserWidgetObject()->GetIsVisible())
//	//	HealthWidget->GetUserWidgetObject()->SetVisibility(ESlateVisibility::HitTestInvisible);
//
//	//return DamageValue;
//
//	//DamageInstigator = EventInstigator;
//	//DamageValue = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
//
//	//State->SetHittedMode();
//	//return Status->GetHealth();
//}

//void ABaseEnemyCharacter::Damaged(float totalAmount)
//{
//	//Status->SubHealth(totalAmount);
//	Cast<UHealthWidget>(HealthWidget->GetUserWidgetObject())->Update(Health, MaxHealth);
//	if (Health <= 0)
//	{
//		Die();
//	}
//	DamageValue = 0;
//}

void ABaseEnemyCharacter::Die()
{
	eCharacterStateFlags = ECharacterStateFlags::DEAD;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	IsFullBody = true;
	IsDie = true;
	CheckNull(BaseAIController);
	BaseAIController->StopAI();
}

void ABaseEnemyCharacter::Elim(bool bPlayerLeftGame)
{
	//DropOrDestroyWeapons();
	MulticastElim(bPlayerLeftGame);
	if (HasAuthority())
	{
		//SpawnPickup();
	}
}

void ABaseEnemyCharacter::MulticastElim_Implementation(bool bPlayerLeftGame)
{
	Die();
	GetWorldTimerManager().SetTimer(
		ElimTimer,
		this,
		&ABaseEnemyCharacter::ElimTimerFinished,
		ElimDelay
	);
}

void ABaseEnemyCharacter::MulticastDamageWidget_Implementation(float Damage)
{
	UWorld* const World = GetWorld();
	if (!IsLocallyControlled())
		return;
	if (World && DamageWidgetClass)
	{
		ADamageFXActor* DamageFXActor = Cast<ADamageFXActor>(World->SpawnActor<AActor>(DamageWidgetClass, FVector::ZeroVector, FRotator::ZeroRotator));

		DamageFXActor->SetDamageText(Damage);

		DamageFXActor->SetActorTransform(GetActorTransform());
		DamageFXActor->SetDamageWidgetSizeAndLocation(FVector::ZeroVector, FVector2D(120, 20));
		DamageFXActor->SetWidgetActive(true);
	}
}

void ABaseEnemyCharacter::ElimTimerFinished()
{
	Destroy();
	//BattleGameMode = BattleGameMode == nullptr ? GetWorld()->GetAuthGameMode<ABattleGameMode>() : BattleGameMode;
	//if (BattleGameMode && !bLeftGame)
	//{
	//	BattleGameMode->RequestRespawn(this, Controller);
	//}
	//if (bLeftGame && IsLocallyControlled())
	//{
	//	OnLeftGame.Broadcast();
	//}
}

void ABaseEnemyCharacter::Init()
{
	//Status->AddHealth(100);
	//	Cast<UHealthWidget>(HealthWidget->GetUserWidgetObject())->Update(Health, MaxHealth);
	//HealthWidget->GetUserWidgetObject()->SetVisibility(ESlateVisibility::Collapsed);
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
	if (HasAuthority())
	{
		OnReturnSpawner.Execute(this);
	}
	//CheckNull(PoolObject);
	//PoolObject->OnReturnToPool.Execute(this);
}


void ABaseEnemyCharacter::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser)
{
	BattleGameMode = BattleGameMode == nullptr ? GetWorld()->GetAuthGameMode<ABattleGameMode>() : BattleGameMode;
	if (BattleGameMode == nullptr) return;
	Damage = BattleGameMode->CalculateDamage(InstigatorController, Controller, Damage);

	float DamageToHealth = Damage;
	if (Shield > 0.f)
	{
		if (Shield >= Damage)
		{
			Shield = FMath::Clamp(Shield - Damage, 0.f, MaxShield);
			DamageToHealth = 0.f;
		}
		else
		{
			DamageToHealth = FMath::Clamp(DamageToHealth - Shield, 0.f, Damage);
			Shield = 0.f;
		}
	}

	Health = FMath::Clamp(Health - DamageToHealth, 0.f, MaxHealth);

	Cast<UHealthWidget>(HealthWidget->GetUserWidgetObject())->Update(Health, MaxHealth);
	/*UpdateHUDHealth();
	UpdateHUDShield();*/
	//PlayHitReactMontage();

	MulticastDamageWidget(Damage);

	if (Health == 0.f)
	{
		if (BattleGameMode)
		{
			AIController = AIController == nullptr ? Cast<ABaseAIController>(Controller) : AIController;
			AInGamePlayerController* AttackerController = Cast<AInGamePlayerController>(InstigatorController);
			BattleGameMode->EnemyEliminated(this, AIController, AttackerController);
		}
	}
}