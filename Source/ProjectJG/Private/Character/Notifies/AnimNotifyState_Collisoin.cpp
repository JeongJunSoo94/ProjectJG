#include "Character/Notifies/AnimNotifyState_Collisoin.h"
#include "Global.h"
#include "Character/Interface/CollisionStateNotifiable.h"

FString UAnimNotifyState_Collisoin::GetNotifyName_Implementation() const
{
	return "Collision";
}

void UAnimNotifyState_Collisoin::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ICollisionStateNotifiable* collisionState = Cast<ICollisionStateNotifiable>(MeshComp->GetOwner());
	CheckNull(collisionState);

	collisionState->CollisionStateNotifyBegin();
}

void UAnimNotifyState_Collisoin::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ICollisionStateNotifiable* collisionState = Cast<ICollisionStateNotifiable>(MeshComp->GetOwner());
	CheckNull(collisionState);

	collisionState->CollisionStateNotifyEnd();

}

