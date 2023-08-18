#include "Character/LtBelica/EruptionHologram.h"
#include "Global.h"
#include "Components/StaticMeshComponent.h"
#include "Components/DecalComponent.h"
#include "Components/BoxComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMaterialLibrary.h"


AEruptionHologram::AEruptionHologram()
{
	CHelpers::CreateComponent<UBoxComponent>(this, &Box, "Box");
	CHelpers::GetAsset<UMaterialInstanceConstant>(&DecalMaterial, "MaterialInstanceConstant'/Game/Developers/JJS/LtBelica/FX/ProjectilePath/M_EruptionDecal_Inst.M_EruptionDecal_Inst'");
	//Box->bHiddenInGame = false;

}

void AEruptionHologram::BeginPlay()
{
	Super::BeginPlay();
	Decal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), DecalMaterial, GetActorScale() * FVector(55), GetActorLocation());

	SetActive(false);
}

void AEruptionHologram::Tick(float DeltaTime)
{

}

void AEruptionHologram::SetHologramLocation(FVector location)
{
	SetActorLocation(location);
	Decal->SetWorldLocation(location);
}

void AEruptionHologram::SetHologramRotator(FRotator rotator)
{
	FRotator box, decal;
	box = Box->GetRelativeRotation();
	box.Yaw = rotator.Yaw;
	SetActorRotation(box);
	decal = rotator;
	decal.Pitch = Decal->GetRelativeRotation().Pitch;
	Decal->SetRelativeRotation(decal);
}

void AEruptionHologram::SetHologramScale(FVector scale)
{
	Box->SetBoxExtent(FVector(220.0f, 55.0f, 135.0f)* scale);
	Decal->SetRelativeScale3D(FVector(2.5f, 1.0f, 4.0f)* scale);
}

void AEruptionHologram::SetActive(bool isActive)
{
	SetActorHiddenInGame(!isActive);
	Decal->SetHiddenInGame(!isActive);
}
FVector AEruptionHologram::GetHologramBoxScale()
{
	return Box->GetScaledBoxExtent();
}
FVector AEruptionHologram::GetHologramDecalScale()
{ 
	return Decal->GetRelativeScale3D(); 
}