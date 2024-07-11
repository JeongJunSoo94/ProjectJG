// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldObjects/Item/Item.h"
#include "AmmoType.h"
#include "Ammo.generated.h"

//USTRUCT(BlueprintType)
//struct FAmmoDataTable : public FTableRowBase
//{
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//		USkeletalMesh* ItemMesh;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//		FString ItemName;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//		UTexture2D* AmmoIcon;
//};


UCLASS()
class PROJECTJG_API AAmmo : public AItem
{
	GENERATED_BODY()
public:
	AAmmo();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

	/** Override of SetItemProperties so we can set AmmoMesh properties */
	virtual void SetItemProperties(EItemState State) override;

	UFUNCTION()
		void AmmoSphereOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

private:
	/** Mesh for the ammo pickup */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ammo, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* AmmoMesh;

	/** Ammo type for the ammo */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo, meta = (AllowPrivateAccess = "true"))
		EAmmoType AmmoType;

	/** The texture for the Ammo icon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo, meta = (AllowPrivateAccess = "true"))
		UTexture2D* AmmoIconTexture;

	/** Overlap sphere for picking up the Ammo */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ammo, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* AmmoCollisionSphere;

public:
	FORCEINLINE UStaticMeshComponent* GetAmmoMesh() const { return AmmoMesh; }
	FORCEINLINE EAmmoType GetAmmoType() const { return AmmoType; }

	virtual void EnableCustomDepth(bool bEnable) override;
};