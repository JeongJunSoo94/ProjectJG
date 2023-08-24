#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemyCharacter.generated.h"

UCLASS()
class PROJECTJG_API ABaseEnemyCharacter : public ACharacter
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
	float TakeDamage(float Damage);
protected:
	virtual void BeginPlay() override;
	void SetHealthWidgetSizeAndLocation(FVector location, FVector2D size);
	virtual void Damaged(float totalAmount);
public:	
	virtual void Tick(float DeltaTime) override;

protected:
	float DamageValue;

};
