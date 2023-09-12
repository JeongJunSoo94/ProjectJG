#include "Character/Notifies/AnimNotify_MiddleAction.h"
#include "Global.h"
#include "Character/CBaseCharacter.h"
#include "Character/Interface/ActionNotifiable.h"

FString UAnimNotify_MiddleAction::GetNotifyName_Implementation() const
{
	return  NotifyName;
}

void UAnimNotify_MiddleAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	IActionNotifiable* actionNotify = Cast<IActionNotifiable>(MeshComp->GetOwner());
	CheckNull(actionNotify);

	actionNotify->MiddleNotifyAction();
}
