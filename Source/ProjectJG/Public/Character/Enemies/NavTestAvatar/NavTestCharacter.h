
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NavTestCharacter.generated.h"

UENUM()
enum class LookPlayerState : uint8
{
	EOutRange = 0				UMETA(DisplayName = "OutRange"),
	ELook						UMETA(DisplayName = "Look"),
	ENotLook					UMETA(DisplayName = "NotLook"),
	EMAX						UMETA(DisplayName = "Max")
};


UCLASS()
class PROJECTJG_API ANavTestCharacter : public ACharacter
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Nav")
		class UNavMovementComponent* NavMoveComp;
	UPROPERTY(VisibleDefaultsOnly, Category = "DetectSphere")
		class USphereComponent* Sphere;
public:

	ANavTestCharacter();

protected:

	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	FTimerHandle RayTimer;

	void RayToPlayers();
	void UpdateAggro(float DeltaTime);
public:
	TMap<class ACBaseCharacter*, LookPlayerState> TargetActors;
	TMap<class ACBaseCharacter*, float> ActorsAggro;


	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	FCollisionQueryParams CollisionParams;
};
