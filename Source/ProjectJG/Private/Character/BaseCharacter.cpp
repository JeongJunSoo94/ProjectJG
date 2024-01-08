#include "Character/BaseCharacter.h"
#include "Global.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::MoveForward(float Axis)
{
	//CheckFalse(bMove);
	if ((Controller != nullptr) && (Axis != 0.0f))
	{
		const FRotator YawRotation{ 0, Controller->GetControlRotation().Yaw, 0 };
		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::X) };
		AddMovementInput(Direction, Axis);
	}
}

void ABaseCharacter::MoveRight(float Axis) 
{
}
void ABaseCharacter::TurnAtRate(float Rate) 
{
}
void ABaseCharacter::LookUpAtRate(float Rate) 
{
}
void ABaseCharacter::Turn(float Value) 
{
}
void ABaseCharacter::LookUp(float Value) 
{
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

