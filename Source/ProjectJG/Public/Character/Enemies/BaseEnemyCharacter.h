#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character/Interface/Damageable.h"
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
class PROJECTJG_API ABaseEnemyCharacter : public ACharacter, public IDamageable
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleDefaultsOnly)
		class UStatusComponent* Status;
	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* HealthWidget;
	UPROPERTY(EditAnywhere)
		ECharacterStateFlags eCharacterStateFlags;
public:
	ABaseEnemyCharacter();
	virtual float TakeDamage(float Damage)override;
protected:
	virtual void BeginPlay() override;
	void SetHealthWidgetSizeAndLocation(FVector location, FVector2D size);
	virtual void Damaged(float totalAmount);
	virtual void Die();
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void BeginHitEffect(FVector NormalImpulse, const FHitResult& Hit) override;
	ECharacterStateFlags GetECharacterStateFlags() { return eCharacterStateFlags; }

	virtual void RegistBlackBoardDatas(class UBlackboardComponent* blackboard);
protected:
	float DamageValue;
public:
	bool isFullBody;
};
