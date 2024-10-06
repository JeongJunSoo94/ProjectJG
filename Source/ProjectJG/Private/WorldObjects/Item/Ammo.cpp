#include "WorldObjects/Item/Ammo.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "Character/BaseCharacter.h"

AAmmo::AAmmo()
{
	// Construct the AmmoMesh component and set it as the root
	AmmoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AmmoMesh"));
	SetRootComponent(AmmoMesh);

	GetCollisionBox()->SetupAttachment(GetRootComponent());
	GetPickupWidget()->SetupAttachment(GetRootComponent());
	GetAreaSphere()->SetupAttachment(GetRootComponent());

	AmmoCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AmmoCollisionSphere"));
	AmmoCollisionSphere->SetupAttachment(GetRootComponent());
	AmmoCollisionSphere->SetSphereRadius(50.f);
}

void AAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAmmo::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	//const FString WeaponTablePath(TEXT("DataTable'/Game/Developers/JJS/Weapons/WeaponsDataTable/WeaponDataTable.WeaponDataTable'"));
	//UDataTable* WeaponTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *WeaponTablePath));

	//if (WeaponTableObject)
	//{
	//	FWeaponDataTable* WeaponDataRow = nullptr;
	//	switch (WeaponType)
	//	{
	//	case EWeaponType::EWT_SubmachineGun:
	//		WeaponDataRow = WeaponTableObject->FindRow<FWeaponDataTable>(FName("SubmachineGun"), TEXT(""));
	//		break;
	//	case EWeaponType::EWT_AssaultRifle:
	//		WeaponDataRow = WeaponTableObject->FindRow<FWeaponDataTable>(FName("AssaultRifle"), TEXT(""));
	//		break;
	//	case EWeaponType::EWT_Pistol:
	//		WeaponDataRow = WeaponTableObject->FindRow<FWeaponDataTable>(FName("Pistol"), TEXT(""));
	//		break;
	//	}

	//	if (WeaponDataRow)
	//	{
	//		//AmmoType = WeaponDataRow->AmmoType;
	//		Ammo = WeaponDataRow->WeaponAmmo;
	//		MagazineCapacity = WeaponDataRow->MagazingCapacity;
	//		//SetPickupSound(WeaponDataRow->PickupSound);
	//		//SetEquipSound(WeaponDataRow->EquipSound);
	//		GetItemMesh()->SetSkeletalMesh(WeaponDataRow->ItemMesh);
	//		SetItemName(WeaponDataRow->ItemName);
	//		SetIconItem(WeaponDataRow->InventoryIcon);
	//		SetAmmoIcon(WeaponDataRow->AmmoIcon);

	//		SetMaterialInstance(WeaponDataRow->MaterialInstance);
	//		PreviousMaterialIndex = GetMaterialIndex();
	//		GetItemMesh()->SetMaterial(PreviousMaterialIndex, nullptr);
	//		SetMaterialIndex(WeaponDataRow->MaterialIndex);
	//		SetClipBoneName(WeaponDataRow->ClipBoneName);
	//		SetReloadMontageSection(WeaponDataRow->ReloadMontageSection);
	//		GetItemMesh()->SetAnimInstanceClass(WeaponDataRow->AnimBP);
	//		CrosshairsCenter = WeaponDataRow->CrosshairsCenter;
	//		CrosshairsLeft = WeaponDataRow->CrosshairsLeft;
	//		CrosshairsRight = WeaponDataRow->CrosshairsRight;
	//		CrosshairsBottom = WeaponDataRow->CrosshairsBottom;
	//		CrosshairsTop = WeaponDataRow->CrosshairsTop;
	//		AutoFireRate = WeaponDataRow->AutoFireRate;
	//		MuzzleFlash = WeaponDataRow->MuzzleFlash;
	//		FireSound = WeaponDataRow->FireSound;
	//		BoneToHide = WeaponDataRow->BoneToHide;
	//		MuzzleSocketName = WeaponDataRow->MuzzleSocketName;
	//		FireType = WeaponDataRow->FireType;
	//		MainHandSocketName = WeaponDataRow->MainHandSocketName;
	//		SubHandSocketName = WeaponDataRow->SubHandSocketName;
	//		ZoomedFOV = WeaponDataRow->ZoomedFOV;
	//		ZoomInterpSpeed = WeaponDataRow->ZoomInterpSpeed;
	//	}
	//	if (GetMaterialInstance())
	//	{
	//		SetDynamicMaterialInstance(UMaterialInstanceDynamic::Create(GetMaterialInstance(), this));
	//		GetDynamicMaterialInstance()->SetVectorParameterValue(TEXT("FresnelColor"), GetGlowColor());
	//		GetItemMesh()->SetMaterial(GetMaterialIndex(), GetDynamicMaterialInstance());

	//		EnableGlowMaterial();
	//	}
	//}
}

void AAmmo::BeginPlay()
{
	Super::BeginPlay();

	AmmoCollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AAmmo::AmmoSphereOverlap);
}

void AAmmo::SetItemProperties(EItemState State)
{
	Super::SetItemProperties(State);

	//switch (State)
	//{
	//case EItemState::EIS_Pickup:
	//	// Set mesh properties
	//	AmmoMesh->SetSimulatePhysics(false);
	//	AmmoMesh->SetEnableGravity(false);
	//	AmmoMesh->SetVisibility(true);
	//	AmmoMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	//	AmmoMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//	break;
	//case EItemState::EIS_Equipped:
	//	// Set mesh properties
	//	AmmoMesh->SetSimulatePhysics(false);
	//	AmmoMesh->SetEnableGravity(false);
	//	AmmoMesh->SetVisibility(true);
	//	AmmoMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	//	AmmoMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//	break;
	//case EItemState::EIS_Falling:
	//	// Set mesh properties
	//	AmmoMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//	AmmoMesh->SetSimulatePhysics(true);
	//	AmmoMesh->SetEnableGravity(true);
	//	AmmoMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	//	AmmoMesh->SetCollisionResponseToChannel(
	//		ECollisionChannel::ECC_WorldStatic,
	//		ECollisionResponse::ECR_Block);
	//	break;
	//case EItemState::EIS_EquipInterping:
	//	// Set mesh properties
	//	AmmoMesh->SetSimulatePhysics(false);
	//	AmmoMesh->SetEnableGravity(false);
	//	AmmoMesh->SetVisibility(true);
	//	AmmoMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	//	AmmoMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//	break;
	//}
}

void AAmmo::AmmoSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		auto OverlappedCharacter = Cast<ABaseCharacter>(OtherActor);
		if (OverlappedCharacter)
		{
			StartItemCurve(OverlappedCharacter);
			AmmoCollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

void AAmmo::EnableCustomDepth(bool bEnable)
{
	AmmoMesh->SetRenderCustomDepth(bEnable);
}
