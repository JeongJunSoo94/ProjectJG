#pragma once

#include "CoreMinimal.h"
#include "Character/Enemies/AIController/BaseAIController.h"
#include "MeleeEnemyAIController.generated.h"

UCLASS()
class PROJECTJG_API AMeleeEnemyAIController : public ABaseAIController
{
	GENERATED_BODY()

public:
	AMeleeEnemyAIController();
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void StartAI()override;
	virtual void StopAI()override;
	virtual void Initialize()override;
protected:
	virtual void BeginPlay() override;

};
