
#include "Character/Enemies/NavTestAvatar/NavTestCharacter.h"
#include "Global.h"
#include "Character/Animation/CCharacterAnimInstance.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/NavMovementComponent.h"

ANavTestCharacter::ANavTestCharacter()
{
	PrimaryActorTick.bCanEverTick = true;


	/*bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;*/

	GetCharacterMovement()->MaxWalkSpeed = 400.0f;

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/ParagonWraith/Characters/Heroes/Wraith/Meshes/Wraith.Wraith'");
	GetMesh()->SetSkeletalMesh(mesh);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	CHelpers::CreateActorComponent<UNavMovementComponent>(this, &NavMoveComp,"NavComponent");
}


void ANavTestCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	

}


void ANavTestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ANavTestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

