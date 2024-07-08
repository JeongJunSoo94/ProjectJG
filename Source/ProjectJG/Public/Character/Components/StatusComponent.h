#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatusComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTJG_API UStatusComponent : public UActorComponent
{
	GENERATED_BODY()
public:	
	UStatusComponent();
	void AddHealth(float InAmount);
	void SubHealth(float InAmount);
protected:
	/*UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* HealthWidget;*/

	//UPROPERTY(EditAnywhere, Category = "Player Stats")
		float MaxHealth = 100.f;

	//UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "Player Stats")
		float Health = 100.f;

	//UFUNCTION()
	//	void OnRep_Health(float LastHealth);

	//UPROPERTY(EditAnywhere, Category = "Player Stats")
	//	float MaxShield = 100.f;

	//UPROPERTY(ReplicatedUsing = OnRep_Shield, EditAnywhere, Category = "Player Stats")
	//	float Shield = 0.f;

	/*UFUNCTION()
		void OnRep_Shield(float LastShield);*/
protected:
	virtual void BeginPlay() override;
	//virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:	
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }
	FORCEINLINE float GetHealth() { return  Health; }
};
