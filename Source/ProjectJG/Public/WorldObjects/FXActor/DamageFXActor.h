#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseSystem/ObjectPoolFunctions.h"
#include "BaseSystem/BasePooledObject.h"
#include "DamageFXActor.generated.h"

UCLASS()
class PROJECTJG_API ADamageFXActor : public AActor, public IObjectPoolFunctions
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, Category = "Widgets")
		class UWidgetComponent* FloatingDamageWidgetComp;
	UPROPERTY(VisibleDefaultsOnly, Category = "Widgets")
		class UFloatingDamageWidget* FloatingDamageWidget;
public:
	UPROPERTY(VisibleDefaultsOnly)
		class UPoolObjectActorComponent* PoolObject;
public:	
	ADamageFXActor();
	void SetDamageWidgetSizeAndLocation(FVector location, FVector2D size);
	void SetDamageText(float damageValue);
	virtual void Init()override;
	void SetWidgetActive(bool bActive);
	void ReturnPool();
protected:
	virtual void BeginPlay() override;


};
