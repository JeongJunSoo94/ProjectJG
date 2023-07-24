#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CLtBelicaWeapon.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTJG_API UCLtBelicaWeapon : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "SMG")
		class UAnimMontage* FireMontage;
public:	
	UCLtBelicaWeapon();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	class ACharacter* OwnerCharacter;
public:
	void SetOwnerCharacter(ACharacter* character);

	void Begin_Fire();
	void Firing();
	void End_Fire();

private:
	FTimerHandle Timer;
	bool IsEquipped;
};
