#include "Character/CBaseCharacter.h"
#include "Global.h"
#include "Character/Animation/CCharacterAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/Components/StatusComponent.h"
#include "BaseSystem/GameHUD.h"
#include "WorldObjects/FXActor/DamageFXActor.h"

#include "GameFramework/Controller.h"

ACBaseCharacter::ACBaseCharacter()
{
 	PrimaryActorTick.bCanEverTick = true;
	CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm",GetCapsuleComponent());
	CHelpers::CreateComponent<UCameraComponent>(this, &PlayerMainCamera, "Camera", SpringArm);

	CHelpers::CreateActorComponent<UStatusComponent>(this, &StatusComp, "StatusComp");
	//CHelpers::CreateComponent<UWidgetComponent>(this, &HealthWidget, "HealthWidget", GetMesh());
	CHelpers::GetClass<ADamageFXActor>(&DamageWidgetClass, "Blueprint'/Game/Developers/JJS/FXActor/BP_DamageActor.BP_DamageActor_C'");

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;

	/*TSubclassOf<UAnimInstance> animInstance;
	CHelpers::GetClass<UAnimInstance>(&animInstance, "AnimBlueprint'/Game/Developers/USER/Collections/TestCharacter/Revenant/Character/ABP_CPlayer.ABP_CPlayer_C'");
	GetMesh()->SetAnimInstanceClass(animInstance);*/

	SpringArm->SetRelativeLocation(FVector(0, 0, 60));
	SpringArm->TargetArmLength = 200.0f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;

	SpringArm->SocketOffset = FVector(0, 60, 0);

	equipedWeaponIdex = 0;
	weaponBoneIdexs.Add(0);
}

void ACBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	CHelpers::CheckNullComponent<UCameraComponent>(this,&PlayerMainCamera);
	CHelpers::CheckNullComponent<USpringArmComponent>(this, &SpringArm);

	AGameHUD* hud = UGameplayStatics::GetPlayerController(this, 0)->GetHUD<AGameHUD>();
	hud->HealthBarUpdate(StatusComp->GetHealth(), StatusComp->GetMaxHealth());

}

void ACBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(DamageValue>0)
		Damaged(DamageValue);
}

void ACBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACBaseCharacter::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACBaseCharacter::OnMoveRight);
	PlayerInputComponent->BindAxis("HorizontalLook", this, &ACBaseCharacter::OnHorizontalLook);
	PlayerInputComponent->BindAxis("VerticalLook", this, &ACBaseCharacter::OnVerticalLook);

	PlayerInputComponent->BindAction("OnEquipNum1", EInputEvent::IE_Pressed, this, &ACBaseCharacter::OnEquipNum1);
	PlayerInputComponent->BindAction("OnUnEquip", EInputEvent::IE_Pressed, this, &ACBaseCharacter::OnUnEquip);
}

void ACBaseCharacter::OnMoveForward(float Axis)
{
	CheckFalse(bMove);
	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetForwardVector().GetSafeNormal2D();
	AddMovementInput(direction, Axis);
}

void ACBaseCharacter::OnMoveRight(float Axis)
{
	CheckFalse(bMove);
	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector().GetSafeNormal2D();
	AddMovementInput(direction, Axis);

}

void ACBaseCharacter::OnHorizontalLook(float Axis)
{
	AddControllerYawInput(Axis);
}

void ACBaseCharacter::OnVerticalLook(float Axis)
{
	AddControllerPitchInput(Axis);
}

void ACBaseCharacter::OnEquipNum1()
{
	Equip(1);
}

void ACBaseCharacter::OnEquipNum2()
{
}

void ACBaseCharacter::OnUnEquip()
{
	UnEquip();
}

void ACBaseCharacter::Equip(int32 weaponIndex)
{
	UnEquip();
	if (weaponIndex != 0)
	{
		GetMesh()->UnHideBone(weaponBoneIdexs[weaponIndex]);
		equipedWeaponIdex = weaponIndex;
	}
	
}

void ACBaseCharacter::UnEquip()
{
	if (equipedWeaponIdex != 0)
	{
		GetMesh()->HideBone(weaponBoneIdexs[equipedWeaponIdex], PBO_None);
		equipedWeaponIdex = 0;
	}
}

float ACBaseCharacter::TakeDamage(float Damage)
{
	DamageValue += Damage;
	return DamageValue;

	//DamageInstigator = EventInstigator;
	//DamageValue = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	//State->SetHittedMode();
	//return Status->GetHealth();
}

void ACBaseCharacter::GetLocationAndDirection(FVector& OutStart, FVector& OutEnd, FVector& OutDirection, bool IsRandom, float MaxYawInDegrees, float MaxPitchInDegrees)
{
	OutDirection = PlayerMainCamera->GetForwardVector();
	FTransform transform = PlayerMainCamera->GetComponentToWorld();
	FVector cameraLocation = transform.GetLocation();
	if (SpringArm == nullptr)
	{
		Clog::Log("SpringArm null");
		OutStart = cameraLocation + OutDirection;
	}
	else
	{
		OutStart = cameraLocation + OutDirection * SpringArm->TargetArmLength;
	}

	FVector conDirection;
	if (IsRandom)
	{
		conDirection = UKismetMathLibrary::RandomUnitVectorInEllipticalConeInDegrees(OutDirection, MaxYawInDegrees, MaxPitchInDegrees);
	}
	else
	{
		conDirection = OutDirection;
	}
	conDirection *= 3000.0f;
	OutEnd = cameraLocation + conDirection;

	FHitResult OutHit;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this->GetOwner());

	//DrawDebugLine(GetWorld(), OutStart, OutEnd, FColor::Blue, false, 2, 0, 2);

	bool IsHit = GetWorld()->LineTraceSingleByChannel(OutHit, OutStart, OutEnd, ECC_Visibility,CollisionParams);
	

	if (IsHit)
	{
		
		OutEnd = OutHit.ImpactPoint;
		//DrawDebugLine(GetWorld(), OutStart, OutEnd, FColor::Blue, false, 1, 0, 1);
	}

}

void ACBaseCharacter::GetLocationAndDirection(FVector muzzleLocation, FVector& OutStart, FVector& OutEnd, FVector& OutDirection, bool IsRandom , float MaxYawInDegrees , float MaxPitchInDegrees )
{
	GetLocationAndDirection(OutStart, OutEnd, OutDirection, IsRandom, MaxYawInDegrees, MaxPitchInDegrees);

	OutDirection = UKismetMathLibrary::GetDirectionUnitVector(muzzleLocation, OutEnd);
}


void ACBaseCharacter::Damaged(float totalAmount)
{
	StatusComp->SubHealth(totalAmount);

	//StatusUI->Update(Status->GetHealth(), Status->GetMaxHealth());

	//HUDComp->HealthBarUpdate(StatusComp->GetHealth(), StatusComp->GetMaxHealth());
	AGameHUD* hud = UGameplayStatics::GetPlayerController(this, 0)->GetHUD<AGameHUD>();
	hud->HealthBarUpdate(StatusComp->GetHealth(), StatusComp->GetMaxHealth());

	DamageValue = 0;
}

void ACBaseCharacter::Stop()
{
	bMove = false;
}
void ACBaseCharacter::SolveStop()
{
	bMove = true;
}

float ACBaseCharacter::GetLookYaw()
{
	// 카메라의 시선 벡터 얻기
	FVector CameraForward = PlayerMainCamera->GetForwardVector();

	// 액터의 시선 벡터 얻기 (예시로 액터의 앞 방향 벡터로 가정)
	FVector ActorForward = GetActorForwardVector();

	// 카메라와 액터의 시선 각도 계산
	FRotator CameraRot = UKismetMathLibrary::MakeRotFromX(CameraForward);
	FRotator ActorRot = UKismetMathLibrary::MakeRotFromX(ActorForward);

	// Yaw 각도 계산
	float YawDifference = UKismetMathLibrary::NormalizedDeltaRotator(CameraRot, ActorRot).Yaw;

	return YawDifference;
}

void ACBaseCharacter::BeginHitEffect(AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{

}

void ACBaseCharacter::SetImpactVectorFrom(FVector& ProjectileVector)
{

}

void ACBaseCharacter::Die()
{}