#include "Character/CBaseCharacter.h"
#include "Global.h"
#include "Character/Animation/CCharacterAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Widgets/UserWidget_CrossHair.h"

ACBaseCharacter::ACBaseCharacter()
{
 	PrimaryActorTick.bCanEverTick = true;
	CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm",GetCapsuleComponent());
	CHelpers::CreateComponent<UCameraComponent>(this, &PlayerMainCamera, "Camera", SpringArm);

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

}

void ACBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	CHelpers::CheckNullComponent<UCameraComponent>(this,&PlayerMainCamera);

	CrossHair = CreateWidget<UUserWidget_CrossHair, APlayerController>(GetController<APlayerController>(), CrossHairClass);
	CrossHair->AddToViewport();
	CrossHair->SetVisibility(ESlateVisibility::Visible);
}

void ACBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
void ACBaseCharacter::GetLocationAndDirection(FVector& OutStart, FVector& OutEnd, FVector& OutDirection)
{
	OutDirection = PlayerMainCamera->GetForwardVector();
	FTransform transform = PlayerMainCamera->GetComponentToWorld();
	FVector cameraLocation = transform.GetLocation();
	OutStart = cameraLocation + OutDirection;

	FVector conDirection = UKismetMathLibrary::RandomUnitVectorInEllipticalConeInDegrees(OutDirection, 0.3f, 0.4f);

	conDirection *= 3000.0f;
	OutEnd = cameraLocation + conDirection;
}