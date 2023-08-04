#include "Character/Notifies/MurdockShotGunEnd_AnimNotify.h"
#include "Global.h"
#include "Character/CBaseCharacter.h"
#include "Character/Murdock/Murdock.h"

FString UMurdockShotGunEnd_AnimNotify::GetNotifyName_Implementation() const
{
	return  "EndMurdockShotGun";
}


void UMurdockShotGunEnd_AnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());
	if (cachingMurdock == nullptr)
	{
		cachingMurdock = Cast<AMurdock>(MeshComp->GetOwner());
	}

	CheckNull(cachingMurdock);
	cachingMurdock->EndShotGunFireAnim();
	
}
