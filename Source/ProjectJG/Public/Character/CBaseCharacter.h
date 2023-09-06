#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character/Interface/Damageable.h"
#include "CBaseCharacter.generated.h"

UCLASS()
class PROJECTJG_API ACBaseCharacter : public ACharacter, public IDamageable
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Camera")
		class UCameraComponent* PlayerMainCamera;
	UPROPERTY(VisibleDefaultsOnly)
		TArray<int32> weaponBoneIdexs;
	UPROPERTY(BlueprintReadOnly,VisibleDefaultsOnly, Category = "equipedWeaponIdex")
		int32 equipedWeaponIdex;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<class UUserWidget_CrossHair> CrossHairClass;

	class UUserWidget_CrossHair* CrossHair;

	UPROPERTY(VisibleDefaultsOnly)
		class UStatusComponent* Status;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<class UStatusUserWidget> StatusClass;

	class UStatusUserWidget* StatusUI;

	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* HealthWidget;
public:
	ACBaseCharacter();
	//float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual float TakeDamage(float Damage)override;

protected:
	virtual void BeginPlay() override;
	virtual void Equip(int32 weaponIndex);
	virtual void UnEquip();
	virtual void Damaged(float totalAmount);
	virtual void Die();

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
	void GetLocationAndDirection(FVector& OutStart, FVector& OutEnd, FVector& OutDirection, bool IsRandom = false, float MaxYawInDegrees = 0.0f, float MaxPitchInDegrees = 0.0f);
	void GetLocationAndDirection(FVector muzzleLocation,FVector& OutStart, FVector& OutEnd, FVector& OutDirection, bool IsRandom = false, float MaxYawInDegrees = 0.0f, float MaxPitchInDegrees = 0.0f);
	void Stop();
	void SolveStop();

	float GetLookYaw();

	virtual void BeginHitEffect(FVector NormalImpulse, const FHitResult& Hit) override;
private:
	bool bMove = true;
	float DamageValue;
};


/*
UPROPERTY(VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Camera")
		class UCameraComponent* PlayerMainCamera;
	UPROPERTY(VisibleDefaultsOnly)
		TArray<int32> weaponBoneIdexs;
	UPROPERTY(BlueprintReadOnly,VisibleDefaultsOnly, Category = "equipedWeaponIdex")
		int32 equipedWeaponIdex;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<class UUserWidget_CrossHair> CrossHairClass;

	class UUserWidget_CrossHair* CrossHair;

	UPROPERTY(VisibleDefaultsOnly)
		class UStatusComponent* Status;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<class UStatusUserWidget> StatusClass;

	class UStatusUserWidget* StatusUI;

	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* HealthWidget;
public:
	ACBaseCharacter();

	float TakeDamage(float Damage);

protected:
	virtual void BeginPlay() override;
	virtual void Equip(int32 weaponIndex);
	virtual void UnEquip();
	virtual void Damaged(float totalAmount);

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

	void Stop();
	void SolveStop();

	float GetLookYaw();
private:
	bool bMove = true;
	float DamageValue;


*/