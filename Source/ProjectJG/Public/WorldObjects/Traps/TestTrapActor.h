#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestTrapActor.generated.h"

UCLASS()
class PROJECTJG_API ATestTrapActor : public AActor
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "BehaviorTree")
		class UBehaviorTreeComponent* BTComp;
	UPROPERTY(VisibleDefaultsOnly, Category = "BehaviorTree")
		class UBehaviorTree* BT;


private:
	UPROPERTY(VisibleDefaultsOnly, Category = "SMG")
		TSubclassOf<class ACBullet> BulletClass;
	UPROPERTY(VisibleDefaultsOnly)
		class UObjectPoolFactory* ObjectPoolFactory;
	UFUNCTION()
		void OnHitPlayer(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:	
	ATestTrapActor();

protected:
	virtual void BeginPlay() override;
	void Fire();
public:
	FTimerHandle LifeTimer;
};
