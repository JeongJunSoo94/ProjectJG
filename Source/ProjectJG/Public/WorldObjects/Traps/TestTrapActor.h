#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestTrapActor.generated.h"

UCLASS()
class PROJECTJG_API ATestTrapActor : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "SMG")
		TSubclassOf<class ACBullet> BulletClass;
	UPROPERTY(VisibleDefaultsOnly)
		class UObjectPoolFactory* ObjectPoolFactory;
	UFUNCTION()
		void OnHitPlayer(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UPROPERTY(VisibleDefaultsOnly)
		class UBehaviorTreeComponent* BTC;
	UPROPERTY(VisibleDefaultsOnly)
		class UBlackboardComponent* BlC;
	UPROPERTY(VisibleDefaultsOnly, Category = "BT")
		class UBehaviorTree* BT;
	UPROPERTY(VisibleDefaultsOnly, Category = "BT")
		class UBlackboardData* BTData;

public:	
	ATestTrapActor();

protected:
	virtual void BeginPlay() override;
	void Fire();
public:
	FTimerHandle LifeTimer;
};
