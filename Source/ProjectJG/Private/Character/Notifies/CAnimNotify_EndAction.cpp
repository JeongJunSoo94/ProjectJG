#include "Character/Notifies/CAnimNotify_EndAction.h"
#include "Global.h"
#include "Character/CBaseCharacter.h"
#include "Character/Interface/ActionNotifiable.h"

FString UCAnimNotify_EndAction::GetNotifyName_Implementation() const
{
	return  "EndNotifyAction";
}

void UCAnimNotify_EndAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	IActionNotifiable* actionNotify = Cast<IActionNotifiable>(MeshComp->GetOwner());
	CheckNull(actionNotify);

	actionNotify->EndNotifyAction();
}