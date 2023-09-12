#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CollisionStateNotifiable.generated.h"

UINTERFACE(MinimalAPI)
class UCollisionStateNotifiable : public UInterface
{
	GENERATED_BODY()
};

class PROJECTJG_API ICollisionStateNotifiable
{
	GENERATED_BODY()

public:
	virtual void CollisionStateNotifyBegin() = 0;
	virtual void CollisionStateNotifyEnd() = 0;
};
