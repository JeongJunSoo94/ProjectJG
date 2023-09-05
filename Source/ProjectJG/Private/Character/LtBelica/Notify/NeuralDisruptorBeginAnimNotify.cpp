#include "Character/LtBelica/Notify/NeuralDisruptorBeginAnimNotify.h"
#include "Global.h"
#include "Character/CBaseCharacter.h"
#include "Character/Components/CActionComponent.h"
#include "Character/LtBelica/RAbilityActionComponent.h"

FString UNeuralDisruptorBeginAnimNotify::GetNotifyName_Implementation() const
{
	return  "NeuralDisruptorBeginNotifyAction";
}


void UNeuralDisruptorBeginAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	URAbilityActionComponent* action = CHelpers::GetComponent<URAbilityActionComponent>(MeshComp->GetOwner());

	CheckNull(action);
	action->BeginNotifyAction();
	//CheckNull(action);
	// action->GetCurrent()->GetDoAction()->Begin_DoAction();
}
