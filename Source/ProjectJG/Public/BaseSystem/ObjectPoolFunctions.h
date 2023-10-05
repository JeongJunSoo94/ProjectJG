#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ObjectPoolFunctions.generated.h"


DECLARE_DELEGATE_OneParam(FOnReturnSpawner, AActor*);

UINTERFACE(MinimalAPI)
class UObjectPoolFunctions : public UInterface
{
	GENERATED_BODY()
};

class PROJECTJG_API IObjectPoolFunctions
{
	GENERATED_BODY()

public:
	FOnReturnSpawner OnReturnSpawner;
	virtual void Init()=0;
};
