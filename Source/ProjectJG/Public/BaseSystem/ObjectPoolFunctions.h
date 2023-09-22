#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ObjectPoolFunctions.generated.h"

UINTERFACE(MinimalAPI)
class UObjectPoolFunctions : public UInterface
{
	GENERATED_BODY()
};

class PROJECTJG_API IObjectPoolFunctions
{
	GENERATED_BODY()

public:
	virtual void Init()=0;
};
