#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character/Interface/Damageable.h"
#include "BaseEnemyCharacter.generated.h"

UCLASS()
class PROJECTJG_API ABaseEnemyCharacter : public ACharacter, public IDamageable
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleDefaultsOnly)
		class UStatusComponent* Status;
protected:
	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* HealthWidget;

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

protected:
	float DamageValue;
public:
	bool isFullBody;
};
