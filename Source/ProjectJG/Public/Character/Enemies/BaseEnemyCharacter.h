#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character/Interface/Damageable.h"
#include "BaseEnemyCharacter.generated.h"

UCLASS()
class PROJECTJG_API ABaseEnemyCharacter : public ACharacter, public IDamageable
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleDefaultsOnly)
		class UStatusComponent* Status;
	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* HealthWidget;
	UPROPERTY(VisibleDefaultsOnly)
		TMap<FName,class UAnimMontage*> BaseMontages;


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
	virtual void BeginHitEffect(AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit) override;
	virtual void SetImpactVectorFrom(FVector& ProjectileVector) override;
	virtual void RegistBlackBoardDatas(class UBlackboardComponent* blackboard);
protected:
	float DamageValue;
public:
	bool isFullBody;
	bool isDie = false;
};
