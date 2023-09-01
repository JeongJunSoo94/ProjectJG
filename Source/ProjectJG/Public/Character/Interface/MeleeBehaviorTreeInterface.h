#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MeleeBehaviorTreeInterface.generated.h"

UINTERFACE(MinimalAPI)
class UMeleeBehaviorTreeInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECTJG_API IMeleeBehaviorTreeInterface
{
	GENERATED_BODY()

public:
	virtual void MeleeAttack()=0;
};
