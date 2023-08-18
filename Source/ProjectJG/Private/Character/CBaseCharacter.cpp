#include "Character/CBaseCharacter.h"
#include "Global.h"
#include "Character/Animation/CCharacterAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/Components/StatusComponent.h"
#include "Widgets/UserWidget_CrossHair.h"
#include "Widgets/StatusUserWidget.h"
#include "Widgets/HealthWidget.h"
#include "Components/WidgetComponent.h"

ACBaseCharacter::ACBaseCharacter()
{
 	PrimaryActorTick.bCanEverTick = true;
	CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm",GetCapsuleComponent());
	CHelpers::CreateComponent<UCameraComponent>(this, &PlayerMainCamera, "Camera", SpringArm);
	
	CHelpers::CreateActorComponent<UStatusComponent>(this, &Status, "Status");
	//CHelpers::CreateComponent<UWidgetComponent>(this, &HealthWidget, "HealthWidget", GetMesh());

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

	CHelpers::GetClass<UUserWidget_CrossHair>(&CrossHairClass, "WidgetBlueprint'/Game/Developers/USER/Character/WB_CrossHair.WB_CrossHair_C'");
	CHelpers::GetClass<UStatusUserWidget>(&StatusClass, "WidgetBlueprint'/Game/Developers/USER/Character/WB_Status.WB_Status_C'");

	CHelpers::CreateComponent<UWidgetComponent>(this, &HealthWidget, "HealthWidget", GetMesh());
	
	TSubclassOf<UHealthWidget> healthClass;
	CHelpers::GetClass<UHealthWidget>(&healthClass, "WidgetBlueprint'/Game/Developers/USER/Character/WB_Health.WB_Health_C'");
	HealthWidget->SetWidgetClass(healthClass);
	HealthWidget->SetRelativeLocation(FVector(0, 0, 200));
	HealthWidget->SetDrawSize(FVector2D(120, 20));
	HealthWidget->SetWidgetSpace(EWidgetSpace::Screen);

}

void ACBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	CHelpers::CheckNullComponent<UCameraComponent>(this,&PlayerMainCamera);

	CrossHair = CreateWidget<UUserWidget_CrossHair, APlayerController>(GetController<APlayerController>(), CrossHairClass);
	CrossHair->AddToViewport();
	CrossHair->SetVisibility(ESlateVisibility::Visible);

	//player ¿œ∂ß
	StatusUI = CreateWidget<UStatusUserWidget, APlayerController>(GetController<APlayerController>(), StatusClass);
	StatusUI->AddToViewport();
	StatusUI->SetVisibility(ESlateVisibility::Visible);

	StatusUI->Update(Status->GetHealth(), Status->GetMaxHealth());

	HealthWidget->InitWidget();
	Cast<UHealthWidget>(HealthWidget->GetUserWidgetObject())->Update(Status->GetHealth(), Status->GetMaxHealth());


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
	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetForwardVector().GetSafeNormal2D();
	AddMovementInput(direction, Axis);
}

void ACBaseCharacter::OnMoveRight(float Axis)
{
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
	OutStart = cameraLocation + OutDirection;

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


	//DrawDebugLine(GetWorld(), OutStart, OutEnd, FColor::Red, false, 1, 0, 1);

	bool IsHit = GetWorld()->LineTraceSingleByChannel(OutHit, OutStart, OutEnd, ECC_Visibility,CollisionParams);
	
	if (IsHit)
	{
		OutEnd = OutHit.ImpactPoint;
		//DrawDebugLine(GetWorld(), OutStart, OutEnd, FColor::Blue, false, 1, 0, 1);
	}
}

void ACBaseCharacter::Damaged(float totalAmount)
{
	Status->SubHealth(totalAmount);

	StatusUI->Update(Status->GetHealth(), Status->GetMaxHealth());

	Cast<UHealthWidget>(HealthWidget->GetUserWidgetObject())->Update(Status->GetHealth(), Status->GetMaxHealth());

	DamageValue = 0;
}