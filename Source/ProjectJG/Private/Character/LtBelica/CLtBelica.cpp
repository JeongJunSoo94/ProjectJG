#include "Character/LtBelica/CLtBelica.h"
#include "Global.h"
#include "Character/LtBelica/CLtBelicaWeapon.h"
#include "Camera/CameraComponent.h"
#include "Character/LtBelica/CQAbliltyActionComponent.h"
#include "Character/LtBelica/EAbliltyActionComponent.h"
#include "Character/LtBelica/RAbilityActionComponent.h"
#include "BaseSystem/GameHUD.h"

ACLtBelica::ACLtBelica()
{
	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/ParagonLtBelica/Characters/Heroes/Belica/Meshes/Belica.Belica'");
	GetMesh()->SetSkeletalMesh(mesh);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	weaponBoneIdexs.Add(GetMesh()->GetBoneIndex("weapon"));
	weaponBoneIdexs.Add(GetMesh()->GetBoneIndex("pistol"));
	equipedSubWeaponIdex = 0;

	CHelpers::CreateActorComponent<UCLtBelicaWeapon>(this, &LtBelicaWeapon,"LtBelicaWeapon");
	LtBelicaWeapon->SetOwnerCharacter(this);

	CHelpers::CreateActorComponent<UCQAbliltyActionComponent>(this, &LtBelicaQAbility, "LtBelicaQAbility");
	LtBelicaQAbility->SetOwnerCharacter(this);

	CHelpers::CreateActorComponent<UEAbliltyActionComponent>(this, &LtBelicaEAbility, "LtBelicaEAbility");
	LtBelicaEAbility->SetOwnerCharacter(this);

	CHelpers::CreateActorComponent<URAbilityActionComponent>(this, &LtBelicaRAbility, "LtBelicaRAbility");
	LtBelicaRAbility->SetOwnerCharacter(this);
}

void ACLtBelica::BeginPlay()
{
	Super::BeginPlay();
	eBelicaAbilityState = EBelicaAbilityState::None;
	GetMesh()->HideBone(weaponBoneIdexs[2], PBO_None);
	IsSkilling = false;
	AGameHUD* hud = UGameplayStatics::GetPlayerController(this, 0)->GetHUD<AGameHUD>();
	hud->SetHUDPlayerControllerSkillBind(LtBelicaWeapon, LtBelicaQAbility, LtBelicaEAbility, LtBelicaRAbility);
}

void ACLtBelica::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ACLtBelica::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &ACLtBelica::OnFire);
	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Released, this, &ACLtBelica::OffFire);

	PlayerInputComponent->BindAction("Q", EInputEvent::IE_Pressed, this, &ACLtBelica::OnQAbility);
	PlayerInputComponent->BindAction("E", EInputEvent::IE_Pressed, this, &ACLtBelica::OnEAbility);
	PlayerInputComponent->BindAction("R", EInputEvent::IE_Pressed, this, &ACLtBelica::OnRAbility);
}

void ACLtBelica::OnFire()
{
	switch (eBelicaAbilityState)
	{
		case EBelicaAbilityState::None:
		{
			LtBelicaWeapon->Begin_Fire();
		}
		break;
		default:
		{	
			CheckFalse(IsSkilling);
			EndNotifyAction();
		}
		break;
	}
}

void ACLtBelica::OffFire()
{
	CheckFalse(!IsSkilling);
	switch (eBelicaAbilityState)
	{
		case EBelicaAbilityState::None:
		{
			LtBelicaWeapon->End_Fire();
		}
		break;
		case EBelicaAbilityState::QAbliity:
		{
			LtBelicaQAbility->HologramAction();
		}
		break;
		case EBelicaAbilityState::EAbliity:
		{
			LtBelicaEAbility->HologramAction();
		}
		break;
		case EBelicaAbilityState::RAbliity:
		{
			LtBelicaRAbility->HologramAction();
		}
		break;
	}
	LtBelicaWeapon->End_Fire();
}

void ACLtBelica::OnQAbility()
{
	CheckFalse(!IsSkilling);
	if (LtBelicaQAbility->GetIsCoolTiming())
		return;
	eBelicaAbilityState = eBelicaAbilityState == EBelicaAbilityState::QAbliity ? EBelicaAbilityState::None : EBelicaAbilityState::QAbliity;
	LtBelicaQAbility->OnStartAction();
}

void ACLtBelica::OnEAbility()
{
	CheckFalse(!IsSkilling);
	if (LtBelicaEAbility->GetIsCoolTiming())
		return;
	eBelicaAbilityState = eBelicaAbilityState == EBelicaAbilityState::EAbliity? EBelicaAbilityState::None : EBelicaAbilityState::EAbliity;
	LtBelicaEAbility->OnStartAction();
}

void ACLtBelica::OnRAbility()
{
	CheckFalse(!IsSkilling);
	if (LtBelicaRAbility->GetIsCoolTiming())
		return;
	eBelicaAbilityState = eBelicaAbilityState == EBelicaAbilityState::RAbliity ? EBelicaAbilityState::None : EBelicaAbilityState::RAbliity;
	GetMesh()->UnHideBone(weaponBoneIdexs[2]);
	LtBelicaRAbility->OnStartAction();
}

bool ACLtBelica::GetLtBelicaWeaponIsFiring()
{
	return LtBelicaWeapon->GetIsFiring();
}

bool ACLtBelica::GetLtBelicaIsAbiliting()
{
	return LtBelicaQAbility->GetIsAbiliting();
}

UCActionComponent* ACLtBelica::GetActionComponent()
{
	switch (eBelicaAbilityState)
	{
	case EBelicaAbilityState::None:
	{
		return nullptr;
	}
	break;
	case EBelicaAbilityState::Fire:
	{
		return nullptr;
	}
	break;
	case EBelicaAbilityState::QAbliity:
	{
		return LtBelicaQAbility;
	}
	break;
	case EBelicaAbilityState::EAbliity:
	{
		return LtBelicaEAbility;
	}
	break;
	case EBelicaAbilityState::RAbliity:
	{
		return LtBelicaRAbility;
	}
	break;
	}
	return nullptr;
}

void ACLtBelica::BeginNotifyAction() 
{
	UCActionComponent* actionComp = GetActionComponent();
	CheckNull(actionComp);
	actionComp->BeginNotifyAction();
	IsSkilling = true;
	Stop();
}
void ACLtBelica::MiddleNotifyAction() 
{
	UCActionComponent* actionComp = GetActionComponent();
	CheckNull(actionComp);
	actionComp->MiddleNotifyAction();
}
void ACLtBelica::EndNotifyAction() 
{
	UCActionComponent* actionComp = GetActionComponent();
	CheckNull(actionComp);
	actionComp->EndNotifyAction();
	SolveStop();
	IsSkilling = false;
	eBelicaAbilityState = EBelicaAbilityState::None;
}