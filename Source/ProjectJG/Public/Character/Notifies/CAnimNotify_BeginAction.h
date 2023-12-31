#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_BeginAction.generated.h"

UCLASS()
class PROJECTJG_API UCAnimNotify_BeginAction : public UAnimNotify
{
GENERATED_BODY()
		FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
