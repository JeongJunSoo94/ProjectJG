#pragma once

#include "CoreMinimal.h"
#include "Character/CBaseCharacter.h"
#include "Character/Interface/ActionNotifiable.h"
#include "Character/Components/CActionComponent.h"
#include "CLtBelica.generated.h"

UENUM(BlueprintType)
enum class EBelicaAbilityState : uint8
{
	None = 0,
	Fire = 1,
	QAbliity =2,
	EAbliity =3,
	RAbliity =4,
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
	UCActionComponent* GetActionComponent();
	virtual void BeginNotifyAction()override;
	virtual void MiddleNotifyAction()override;
	virtual void EndNotifyAction()override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	void OnFire();
	void OffFire();
	void OnQAbility();
	void OnEAbility();
	void OnRAbility();
public:
	bool GetLtBelicaWeaponIsFiring();
	bool GetLtBelicaIsAbiliting();
private:
	bool IsSkilling;

};