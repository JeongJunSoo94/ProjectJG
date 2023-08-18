#include "Character/LtBelica/ManaBombHologram.h"
#include "Global.h"
#include "Components/StaticMeshComponent.h"
#include "Components/DecalComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMaterialLibrary.h"

AManaBombHologram::AManaBombHologram()
{
	CHelpers::CreateComponent(this, &Mesh, "Mesh");
	UStaticMesh* mesh;
	CHelpers::GetAsset<UStaticMesh>(&mesh, "StaticMesh'/Game/Developers/JJS/LtBelica/FX/Meshes/SphereManaBomb.SphereManaBomb'");
	Mesh->SetStaticMesh(mesh);

	UMaterialInstanceConstant* material;
	CHelpers::GetAsset<UMaterialInstanceConstant>(&material, "MaterialInstanceConstant'/Game/Developers/JJS/LtBelica/FX/ProjectilePath/M_ManaBombSphere_Inst.M_ManaBombSphere_Inst'");

	Mesh->SetMaterial(0, material);

	CHelpers::GetAsset<UMaterialInstanceConstant>(&DecalMaterial, "MaterialInstanceConstant'/Game/Developers/JJS/LtBelica/FX/ProjectilePath/M_ManaBombDecal_Inst.M_ManaBombDecal_Inst'");

}

void AManaBombHologram::BeginPlay()
{
	Super::BeginPlay();
	Decal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), DecalMaterial, GetActorScale()*FVector(55),GetActorLocation());
	SetActive(false);
}

void AManaBombHologram::Tick(float DeltaTime)
{
}

void AManaBombHologram::SetHologramLocation(FVector location)
{
	SetActorLocation(location);
	Decal->SetWorldLocation(location);
}

void AManaBombHologram::SetHologramScale(FVector scale)
{
	SetActorScale3D(scale);
	Decal->SetWorldScale3D(scale);
}

void AManaBombHologram::SetActive(bool isActive)
{
	SetActorHiddenInGame(!isActive);
	Decal->SetHiddenInGame(!isActive);
}
