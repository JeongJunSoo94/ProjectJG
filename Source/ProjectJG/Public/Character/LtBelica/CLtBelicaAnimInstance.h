#pragma once

#include "CoreMinimal.h"
#include "Character/Animation/CCharacterAnimInstance.h"
#include "CLtBelicaAnimInstance.generated.h"

UCLASS()
class PROJECTJG_API UCLtBelicaAnimInstance : public UCCharacterAnimInstance
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
	class ACLtBelica* LtBelicaCharacter;
};
