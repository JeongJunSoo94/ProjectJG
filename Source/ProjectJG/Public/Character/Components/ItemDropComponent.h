// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "WorldObjects/Item/WeaponType.h"
#include "WorldObjects/Item/Weapon.h"
#include "ItemDropComponent.generated.h"

USTRUCT(BlueprintType)
struct FWeaponSpawnParam
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;
};

//드롭테이블에 사용될 데이터 테이블
USTRUCT(BlueprintType)
struct FWeaponDropDataTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	UDataTable* WeaponTable;
	//데이터 정보

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTJG_API UItemDropComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemDropComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class AItem>> ItemClasses;

public:	
	UFUNCTION()
	void SpawnItem();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void InitailizeItemDropTable();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = DropTable, meta = (AllowPrivateAccess = "true"))
		UDataTable* WeaponDropDataTable;

};
