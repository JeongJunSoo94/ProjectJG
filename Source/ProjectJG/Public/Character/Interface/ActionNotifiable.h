#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ActionNotifiable.generated.h"

UINTERFACE(MinimalAPI)
class UActionNotifiable : public UInterface
{
	GENERATED_BODY()
};

class PROJECTJG_API IActionNotifiable
{
	GENERATED_BODY()

public:
	virtual void BeginNotifyAction() =0;
	virtual void MiddleNotifyAction() =0;
	virtual void EndNotifyAction() = 0;
};
