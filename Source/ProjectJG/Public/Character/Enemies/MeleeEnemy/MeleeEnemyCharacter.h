#pragma once

#include "CoreMinimal.h"
#include "Character/Enemies/BaseEnemyCharacter.h"
#include "MeleeEnemyCharacter.generated.h"

UCLASS()
class PROJECTJG_API AMeleeEnemyCharacter : public ABaseEnemyCharacter
{
	GENERATED_BODY()
public:
	AMeleeEnemyCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
