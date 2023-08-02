#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatusComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTJG_API UStatusComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly, Category = "Health")
		float MaxHealth = 100.0f;

public:	
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }
	FORCEINLINE float GetHealth() { return  Health; }

	UStatusComponent();
private:
	float Health;
protected:
	virtual void BeginPlay() override;

public:	
	void AddHealth(float InAmount);
	void SubHealth(float InAmount);
};
