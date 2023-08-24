
#include "Character/Enemies/NavTestAvatar/NavTestCharacter.h"
#include "Global.h"
#include "Character/Animation/CCharacterAnimInstance.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/NavMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Character/CBaseCharacter.h"


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

	CHelpers::CreateComponent<USphereComponent>(this, &Sphere, "Sphere");
	Sphere->bHiddenInGame = false;
}


void ANavTestCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(RayTimer, this,&ANavTestCharacter::RayToPlayers,1.0f,true);
	CollisionParams.AddIgnoredActor(this->GetOwner());
}

void ANavTestCharacter::NotifyActorBeginOverlap(AActor* OtherActor)
{
	
	if (OtherActor->IsA<ACBaseCharacter>())
	{
		Clog::Log("otherActor is a ACBAseCharacter");
		ACBaseCharacter* player = Cast<ACBaseCharacter>(OtherActor);
		if (TargetActors.Find(player))
		{
			TargetActors[player] = LookPlayerState::ENotLook;
			ActorsAggro[player] = 0.0f;
		}
		else
		{
			TargetActors.Add(Cast<ACBaseCharacter>(OtherActor), LookPlayerState::ENotLook);
			ActorsAggro.Add(Cast<ACBaseCharacter>(OtherActor), 0.0f);
		}

	}
}

void ANavTestCharacter::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (OtherActor->IsA<ACBaseCharacter>())
	{
		TargetActors[Cast<ACBaseCharacter>(OtherActor)] = LookPlayerState::EOutRange;
	}
}

void ANavTestCharacter::RayToPlayers()
{
	for (TPair<ACBaseCharacter*, LookPlayerState>& Elem : TargetActors)
	{
		if (Elem.Value != LookPlayerState::EOutRange)
		{
			FHitResult OutHit;
			FVector Start = GetActorLocation();
			FVector End = Elem.Key->GetActorLocation();

			if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams))
			{
				if (OutHit.GetActor() == Elem.Key)
				{
					Elem.Value = LookPlayerState::ELook;
				}
				else
				{
					Elem.Value = LookPlayerState::ENotLook;
				}
			}
		}
	}
}

void ANavTestCharacter::UpdateAggro(float DeltaTime)
{
	for (TPair<ACBaseCharacter*, float>& Elem : ActorsAggro)
	{
		switch (TargetActors[Elem.Key])
		{
		case LookPlayerState::ELook:
			{
			if (Elem.Value > 100.0f)
			{
				Elem.Value += DeltaTime;
				if (Elem.Value >= 10.0f)
				{
					Elem.Value = 100.0f;
				}
			}
			}
			break;
		case LookPlayerState::ENotLook:
		case LookPlayerState::EOutRange:
			{
			if (Elem.Value > 0.0f)
			{
				Elem.Value -= DeltaTime;

				if (Elem.Value <= 0.0f)
				{
					Elem.Value = 0.0f;
				}
			}
			}
			break;
		default:
			break;
		}
	}
}


void ANavTestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ANavTestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

