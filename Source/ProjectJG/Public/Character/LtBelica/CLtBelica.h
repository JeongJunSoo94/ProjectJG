#pragma once

#include "CoreMinimal.h"
#include "Character/CBaseCharacter.h"
#include "Character/Interface/ActionNotifiable.h"
#include "CLtBelica.generated.h"

UENUM(BlueprintType)
enum class EBelicaAbilityState : uint8
{
	None =0,
	QAbliity =1,
	EAbliity =2,
	RAbliity =3,
};

UCLASS()
class PROJECTJG_API ACLtBelica : public ACBaseCharacter, public IActionNotifiable
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCLtBelicaWeapon* LtBelicaWeapon;
	UPROPERTY(VisibleDefaultsOnly)
		class UCQAbliltyActionComponent* LtBelicaQAbility;
	UPROPERTY(VisibleDefaultsOnly)
		class UEAbliltyActionComponent* LtBelicaEAbility;
	UPROPERTY(VisibleDefaultsOnly)
		class URAbilityActionComponent* LtBelicaRAbility;
	UPROPERTY(EditAnywhere)
		EBelicaAbilityState eBelicaAbilityState;
protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "equipedSubWeaponIdex")
		int32 equipedSubWeaponIdex;

public:
	ACLtBelica();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual UCActionComponent* GetActionComponent() override;
private:
	void OnFire();
	void OffFire();
	void OnQAbility();
	void OnEAbility();
	void OnRAbility();
public:
	bool GetLtBelicaWeaponIsFiring();
	bool GetLtBelicaIsAbiliting();
};