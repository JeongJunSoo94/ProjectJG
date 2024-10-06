#include "Character/Components/ItemDropComponent.h"
#include "Global.h"
#include "Net/UnrealNetwork.h"
#include "WorldObjects/Item/Item.h"

UItemDropComponent::UItemDropComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UItemDropComponent::BeginPlay()
{
	Super::BeginPlay();
	InitailizeItemDropTable();
}

void UItemDropComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UItemDropComponent::InitailizeItemDropTable()
{


}

