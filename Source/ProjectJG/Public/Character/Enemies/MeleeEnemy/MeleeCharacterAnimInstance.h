#pragma once

#include "CoreMinimal.h"
#include "Character/Animation/CCharacterAnimInstance.h"
#include "MeleeCharacterAnimInstance.generated.h"

UCLASS()
class PROJECTJG_API UMeleeCharacterAnimInstance : public UCCharacterAnimInstance
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		bool IsFiring;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		float PistolHolster;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		bool IsQAbiliting;

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
private:
	class AMeleeEnemyCharacter* MeleeCharacter;
};
