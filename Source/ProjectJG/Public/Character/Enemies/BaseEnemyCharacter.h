#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character/Interface/Damageable.h"
#include "BaseSystem/ObjectPoolFunctions.h"
#include "BaseSystem/BasePooledObject.h"
#include "BaseEnemyCharacter.generated.h"

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ECharacterStateFlags : uint8
{
	DEFAULT = 0,
	IDLE = 1 << 0,
	ATTACKING = 1 << 1,
	SKILL = 1 << 2,
	DEAD = 1 << 3,
	SKILLATTACKING = ATTACKING | SKILL,
};
ENUM_CLASS_FLAGS(ECharacterStateFlags);

UCLASS()
class PROJECTJG_API ABaseEnemyCharacter : public ACharacter, public IObjectPoolFunctions
{
	GENERATED_BODY()
protected:
	//UPROPERTY(VisibleDefaultsOnly)
	//	class UStatusComponent* Status;

	//>>status widget
	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* HealthWidget;

	//<<
	UPROPERTY(EditAnywhere)
		ECharacterStateFlags eCharacterStateFlags;
	UPROPERTY(VisibleDefaultsOnly)
		class ABaseAIController* BaseAIController;

	UPROPERTY(VisibleDefaultsOnly, Category = "DamageWidget")
		TSubclassOf<class ADamageFXActor> DamageWidgetClass;

//public:
//	UPROPERTY(VisibleDefaultsOnly)
//		class UPoolObjectActorComponent* PoolObject;
//	UPROPERTY(VisibleDefaultsOnly)
//		class UObjectPoolFactory* ObjectPoolFactory;
public:
	ABaseEnemyCharacter();
	//virtual float TakeDamage(float Damage)override;
	//>>
	void Elim(bool bPlayerLeftGame);
	UFUNCTION(NetMulticast, Reliable)
		void MulticastElim(bool bPlayerLeftGame);
	//<<
protected:
	virtual void BeginPlay() override;
	void SetHealthWidgetSizeAndLocation(FVector location, FVector2D size);
	virtual void Damaged(float totalAmount);
	//>>
	virtual void Die();
	//<<
	//virtual void SetImpactVectorFrom(FVector& ProjectileVector) override;

	UFUNCTION()
		void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);

public:	
	virtual void Tick(float DeltaTime) override;
	//virtual void BeginHitEffect(AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit) override;
	virtual void Init()override;
	void ReturnPool();
	ECharacterStateFlags GetECharacterStateFlags() { return eCharacterStateFlags; }

	virtual void RegistBlackBoardDatas(class UBlackboardComponent* blackboard);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	float DamageValue;
	UPROPERTY(VisibleDefaultsOnly)
		class UBlackboardComponent* blackboardComp;
	UPROPERTY()
		class ABattleGameMode* BattleGameMode;
	//>>status param
	UPROPERTY(EditAnywhere, Category = "Player Stats")
		float MaxHealth = 100.f;

	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "Player Stats")
		float Health = 100.f;

	UFUNCTION()
		void OnRep_Health(float LastHealth);

	UPROPERTY(EditAnywhere, Category = "Player Stats")
		float MaxShield = 100.f;

	UPROPERTY(ReplicatedUsing = OnRep_Shield, EditAnywhere, Category = "Player Stats")
		float Shield = 0.f;

	UFUNCTION()
		void OnRep_Shield(float LastShield);
	//<<

	//>>Elim
	bool bElimmed = false;

	FTimerHandle ElimTimer;

	UPROPERTY(EditDefaultsOnly)
		float ElimDelay = 3.f;

	void ElimTimerFinished();
	//<<

	//>>
	UPROPERTY()
		class ABaseAIController* AIController;
	//<<

public:
	bool IsFullBody;
	bool IsDie;
};
