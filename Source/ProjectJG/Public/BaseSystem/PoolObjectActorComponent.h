#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PoolObjectActorComponent.generated.h"

DECLARE_DELEGATE_OneParam(FOnReturnToPool, AActor*);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTJG_API UPoolObjectActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPoolObjectActorComponent();

	FOnReturnToPool OnReturnToPool;

	UFUNCTION(BlueprintCallable, Category = "ObjectPool")
		void Deactive();
	void SetActive(bool IsActive);
	void SetPoolIndex(int Index, FName ObjectID);
	void SetActorLifeTime(float LifeTime);

	bool IsActive();
	int GetPoolIndex();

protected:
	virtual void BeginPlay() override;
	virtual void Init();

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	float GetCurrentLifeTime();
	float GetActorLifeTime() { return ActorLifeTime; }
	FName GetPoolObjectID() {return PoolObjectID;}
protected:
	bool bPooledObjectActive;
	FName PoolObjectID;
	int PoolObjectIndex;
	float ActorLifeTime = 0.0f;
	FTimerHandle LifeTimer;
};
