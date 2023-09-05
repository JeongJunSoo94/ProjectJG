#include "Character/Notifies/CAnimNotify_BeginAction.h"
#include "Global.h"
#include "Character/CBaseCharacter.h"
#include "Character/Components/CActionComponent.h"
#include "Character/Interface/ActionNotifiable.h"

FString UCAnimNotify_BeginAction::GetNotifyName_Implementation() const
{
	return  "BeginNotifyAction";
}

void UCAnimNotify_BeginAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	IActionNotifiable* actionNotify = Cast<IActionNotifiable>(MeshComp->GetOwner());
	CheckNull(actionNotify);

	UCActionComponent* action = actionNotify->GetActionComponent();
	CheckNull(action);

	action->BeginNotifyAction();
}