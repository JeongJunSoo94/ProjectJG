#include "WorldObjects/Pickup/FireSpeedPickup.h"
#include "Character/BaseCharacter.h"
#include "Character/Components/BuffComponent.h"

void AFireSpeedPickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(OtherActor);
	if (BaseCharacter)
	{
		UBuffComponent* Buff = BaseCharacter->GetBuff();
		if (Buff)
		{
			Buff->BuffFireSpeed(BaseSpeedBuff, SpeedBuffTime);
		}
	}

	Destroy();
}