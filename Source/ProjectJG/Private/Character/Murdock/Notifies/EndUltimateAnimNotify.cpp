
#include "Character/Murdock/Notifies/EndUltimateAnimNotify.h"
#include "Global.h"
#include "Character/CBaseCharacter.h"
#include "Character/Murdock/Murdock.h"

FString UEndUltimateAnimNotify::GetNotifyName_Implementation() const
{
	return  "EndUltimate";
}

void UEndUltimateAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	if (cachingMurdock == nullptr)
	{
		cachingMurdock = Cast<AMurdock>(MeshComp->GetOwner());
	}

	CheckNull(cachingMurdock);
	//cachingMurdock->EndUltimateToIdle();

}