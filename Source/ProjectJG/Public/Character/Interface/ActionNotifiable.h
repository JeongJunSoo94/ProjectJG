#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Character/Components/CActionComponent.h"
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
	virtual UCActionComponent* GetActionComponent() = 0;
};
