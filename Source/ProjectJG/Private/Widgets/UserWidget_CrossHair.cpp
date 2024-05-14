#include "Widgets/UserWidget_CrossHair.h"
#include "GameFramework/CharacterMovementComponent.h"

//UUserWidget_CrossHair::UUserWidget_CrossHair()
//{
//}

void UUserWidget_CrossHair::CalculateCrosshairSpread(float DeltaTime,const FVector PawnVelocity, FVector2D WalkSpeedRange, FVector2D VelocityMultiplierRange)
{
	/*FVector2D WalkSpeedRange{ 0.f,600.f };
	FVector2D VelocityMultiplierRange{ 0.f,1.f };*/
	FVector Velocity{ PawnVelocity };
	Velocity.Z = 0.f;

	CrosshairVelocityFactor = FMath::GetMappedRangeValueClamped(WalkSpeedRange, VelocityMultiplierRange, Velocity.Size());

	if (character->GetCharacterMovement()->IsFalling())
	{
		CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 2.25f, DeltaTime, 2.25f);
	}
	else
	{
		CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 0.f, DeltaTime, 30.f);
	}

	if (bAiming)
	{
		CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor, 0.6f, DeltaTime, 30.f);
	}
	else
	{
		CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor, 0.f, DeltaTime, 30.f);
	}

	if (bFiringBullet)
	{
		CrosshairShootingFactor = FMath::FInterpTo(CrosshairShootingFactor, 0.3f, DeltaTime, 60.f);
	}
	else
	{
		CrosshairShootingFactor = FMath::FInterpTo(CrosshairShootingFactor, 0.f, DeltaTime, 60.f);
	}

	CrosshairSpreadMultiplier = 0.5f + CrosshairVelocityFactor + CrosshairInAirFactor - CrosshairAimFactor + CrosshairShootingFactor;
}

void UUserWidget_CrossHair::StartCrosshairBulletFire()
{
	bFiringBullet = true;
	GetWorld()->GetTimerManager().SetTimer(CrosshairShootTimer, this, &UUserWidget_CrossHair::FinishCrosshairBulletFire, ShootTimeDuration,false);
}

void UUserWidget_CrossHair::FinishCrosshairBulletFire()
{
	bFiringBullet = false;
}