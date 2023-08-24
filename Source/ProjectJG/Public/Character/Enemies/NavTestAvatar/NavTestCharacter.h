
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NavTestCharacter.generated.h"

UCLASS()
class PROJECTJG_API ANavTestCharacter : public ACharacter
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Nav")
		class UNavMovementComponent* NavMoveComp;


public:

	ANavTestCharacter();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
