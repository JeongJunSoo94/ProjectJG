
#include "Character/Murdock/Notifies/MurdockShieldAnimNotify.h"
#include "Global.h"
#include "Character/CBaseCharacter.h"
#include "Character/Components/CActionComponent.h"
#include "Character/Murdock/Murdock.h"

FString UMurdockShieldAnimNotify::GetNotifyName_Implementation() const
{
	return  "BeginShieldAnim";
}

void UMurdockShieldAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());


	if (cachingMurdock == nullptr)
	{
		cachingMurdock = Cast<AMurdock>(MeshComp->GetOwner());
	}

	CheckNull(cachingMurdock);
	//cachingMurdock->LoopShield();
}

