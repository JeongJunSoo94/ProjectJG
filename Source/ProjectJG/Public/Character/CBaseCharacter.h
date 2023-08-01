#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CBaseCharacter.generated.h"

UCLASS()
class PROJECTJG_API ACBaseCharacter : public ACharacter
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;
protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Camera")
		class UCameraComponent* PlayerMainCamera;
	UPROPERTY(VisibleDefaultsOnly)
		TArray<int32> weaponBoneIdexs;
	UPROPERTY(BlueprintReadOnly,VisibleDefaultsOnly, Category = "equipedWeaponIdex")
		int32 equipedWeaponIdex;
public:
	ACBaseCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Equip(int32 weaponIndex);
	virtual void UnEquip();
public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	void OnMoveForward(float Axis);
	void OnMoveRight(float Axis);
	void OnHorizontalLook(float Axis);
	void OnVerticalLook(float Axis);
	void OnEquipNum1();
	void OnEquipNum2();
	void OnUnEquip();
public:
	void GetLocationAndDirection(FVector& OutStart, FVector& OutEnd, FVector& OutDirectiron);
};
