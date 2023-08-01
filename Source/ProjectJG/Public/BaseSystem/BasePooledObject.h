#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePooledObject.generated.h"

DECLARE_DELEGATE_OneParam(FOnReturnedToPool, ABasePooledObject*);

UCLASS()
class PROJECTJG_API ABasePooledObject : public AActor
{
	GENERATED_BODY()
	
public:	
	ABasePooledObject();

	FOnReturnedToPool OnReturnedToPool;

	UFUNCTION(BlueprintCallable, Category = "ObjectPool")
		void Deactive();
	void SetActive(bool IsActive);
	void SetPoolIndex(int Index);
	void SetActorLifeTime(float LifeTime);

	bool IsActive();
	int GetPoolIndex();

protected:
	virtual void BeginPlay() override;
	virtual void Init();

public:	
	virtual void Tick(float DeltaTime) override;
protected:
	bool bPooledObjectActive;
	int PoolObjectIndex;
	float ActorLifeTime = 0.0f;
	FTimerHandle LifeTimer;
};
