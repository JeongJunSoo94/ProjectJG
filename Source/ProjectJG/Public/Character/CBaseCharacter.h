#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character/Interface/Damageable.h"
#include "BaseSystem/GameHUD.h"
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

	UPROPERTY(VisibleDefaultsOnly, Category = "DamageWidget")
		TSubclassOf<class ADamageFXActor> DamageWidgetClass;

	UPROPERTY(VisibleDefaultsOnly)
		class UStatusComponent* StatusComp;
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
	virtual void OnSubMenu();

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	UFUNCTION()
		void StartJump();
	UFUNCTION()
		void StopJump();

	void OnMoveForward(float Axis);
	void OnMoveRight(float Axis);
	void OnHorizontalLook(float Axis);
	void OnVerticalLook(float Axis);
	void OnEquipNum1();
	void OnEquipNum2();
	void OnUnEquip();
public:
	float SetFocusActorYawDegree();
	void GetLocationAndDirection(FVector& OutStart, FVector& OutEnd, FVector& OutDirection, bool IsRandom = false, float MaxYawInDegrees = 0.0f, float MaxPitchInDegrees = 0.0f);
	void GetLocationAndDirection(FVector muzzleLocation,FVector& OutStart, FVector& OutEnd, FVector& OutDirection, bool IsRandom = false, float MaxYawInDegrees = 0.0f, float MaxPitchInDegrees = 0.0f);
	void Stop();
	void SolveStop();

	bool isStop() { return bMove; }

	float GetLookYaw();

	virtual void BeginHitEffect(AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit) override;
	virtual void SetImpactVectorFrom(FVector& ProjectileVector) override;
	FRotator GetAimRotation() {return CharacterRotation;}
private:
	class AGameHUD* GameHUD;
	bool bMove = true;
	float DamageValue;
	FRotator CharacterRotation;
};