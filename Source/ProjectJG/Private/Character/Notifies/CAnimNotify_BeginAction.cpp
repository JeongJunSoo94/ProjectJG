#include "Character/Notifies/CAnimNotify_BeginAction.h"
#include "Global.h"
#include "Character/CBaseCharacter.h"
#include "Character/Components/CActionComponent.h"
//#include "ACtions/CDoAction.h"
//#include "Components/CActionComponent.h"

FString UCAnimNotify_BeginAction::GetNotifyName_Implementation() const
{
	return  "BeginAction";
}


void UCAnimNotify_BeginAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());

	CheckNull(action);
	action->BeginAction();
	//CheckNull(action);
	// action->GetCurrent()->GetDoAction()->Begin_DoAction();
}