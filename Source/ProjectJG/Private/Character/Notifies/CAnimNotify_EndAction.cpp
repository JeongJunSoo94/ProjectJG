#include "Character/Notifies/CAnimNotify_EndAction.h"
#include "Global.h"
#include "Character/CBaseCharacter.h"
#include "Character/Components/CActionComponent.h"
#include "Character/Interface/ActionNotifiable.h"

FString UCAnimNotify_EndAction::GetNotifyName_Implementation() const
{
	return  "EndAction";
}

void UCAnimNotify_EndAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	IActionNotifiable* actionNotify = Cast<IActionNotifiable>(MeshComp->GetOwner());
	CheckNull(actionNotify);

	UCActionComponent* action = actionNotify->GetActionComponent();

	CheckNull(action);
	action->EndAction();
}