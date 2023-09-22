
#include "Character/Murdock/Murdock.h"
#include "Global.h"
#include "Character/CBaseCharacter.h"
#include "Character/Murdock/MurdockWeapon.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Character/LtBelica/CQAbliltyActionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Character/Murdock/MurdockShieldSkillComponent.h"
#include "Character/Murdock/MurdockTazerTrapSkillComponent.h"
#include "Character/Murdock/MurdockSpreadShotSkillComponent.h"
#include "Character/Murdock/MurdockUltimateSkillComponent.h"

#include "Curves/CurveFloat.h"

#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameStateBase.h"
#include "BaseSystem/GameHUD.h"

AMurdock::AMurdock()
{
	PrimaryActorTick.bCanEverTick = true;

	isChangeFOV = false;
	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/ParagonMurdock/Characters/Heroes/Murdock/Meshes/Murdock.Murdock'");
	GetMesh()->SetSkeletalMesh(mesh);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	weaponBoneIdexs.Add(GetMesh()->GetBoneIndex("weapon"));
	CHelpers::CreateActorComponent<UMurdockWeapon>(this, &MurdockWeapon, "MurdockWeapon");
	MurdockWeapon->SetOwnerCharacter(this);


	CHelpers::CreateActorComponent<UMurdockUltimateSkillComponent>(this, &UltimateSkill, "ElevenSkill");
	CHelpers::CreateActorComponent<UMurdockShieldSkillComponent>(this, &ShieldSkill, "ShieldSkill");
	CHelpers::CreateActorComponent<UMurdockTazerTrapSkillComponent>(this, &TazerTrapSkill, "TazerTrapSkill");

	CHelpers::CreateActorComponent<UMurdockSpreadShotSkillComponent>(this, &SpreadShotSkill, "SpreadShotSkill");

	//CHelpers::GetAsset<UCurveFloat>(&ZoomCurveFloat, "CurveFloat'/Game/Developers/GohyeongJu/Characters/Murdock/ZoomCurveBase.ZoomCurveBase'");
	
	ConstructorHelpers::FObjectFinder<UCurveFloat> ZoomCurve(TEXT("CurveFloat'/Game/Developers/GohyeongJu/Characters/Murdock/ZoomCurveBase.ZoomCurveBase'"));
	ZoomCurveFloat = ZoomCurve.Object;
	
	SpreadShotSkill->CreateObjectPool();

}




void AMurdock::BeginPlay()
{
	Super::BeginPlay();
	CHelpers::CheckNullComponent<UMurdockSpreadShotSkillComponent>(this, &SpreadShotSkill);
	CHelpers::CheckNullComponent<UMurdockUltimateSkillComponent>(this, &UltimateSkill);
	currentFOV = PlayerMainCamera->FieldOfView;
	
	AGameHUD* hud = UGameplayStatics::GetPlayerController(this, 0)->GetHUD<AGameHUD>();
	hud->SetHUDPlayerControllerSkillBind(MurdockWeapon, ShieldSkill, SpreadShotSkill, UltimateSkill);
}

void AMurdock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isChangeFOV)
	{
		DoZoom(DeltaTime);
	}
	else
	{
		//Clog::Log("NotZoom");
	}
}

void AMurdock::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &AMurdock::OnFire);
	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Released, this, &AMurdock::OffFire);
	PlayerInputComponent->BindAction("ForwardLook_Move", EInputEvent::IE_Pressed, this, &AMurdock::OnAlt);
	PlayerInputComponent->BindAction("ForwardLook_Move", EInputEvent::IE_Released, this, &AMurdock::OffAlt);
	PlayerInputComponent->BindAction("Q", EInputEvent::IE_Pressed, this, &AMurdock::OnShield);
	PlayerInputComponent->BindAction("Q", EInputEvent::IE_Released, this, &AMurdock::OffShield);
	PlayerInputComponent->BindAction("E", EInputEvent::IE_Pressed, this, &AMurdock::OnSpreadShot);
	PlayerInputComponent->BindAction("E", EInputEvent::IE_Released, this, &AMurdock::OffSpreadShot);
	PlayerInputComponent->BindAction("R", EInputEvent::IE_Pressed, this, &AMurdock::OnUltimate);
	//PlayerInputComponent->BindAction("R", EInputEvent::IE_Released, this, &AMurdock::OffUltimate);
}

void AMurdock::OnFire()
{
	//GetWorld()
	Clog::Log(GetWorld()->GetGameState()->GetName());
	if (BehaviorState == MurdockBehaviorState::EIdle)
	{
		OnStartActionMontage(MurdockBehaviorState::EFire);
		//MurdockWeapon->Begin_Fire();
	}
	else if (BehaviorState == MurdockBehaviorState::EUltimate)
	{
		OnStartActionMontage(MurdockBehaviorState::EUltimate);
		//UltimateSkill->ShotLaser();
	}
	else
	{
		return;
	}

}

void AMurdock::OffFire()
{
	if (BehaviorState == MurdockBehaviorState::EFire)
	{
		OnEndActionMontage();
	}
}

void AMurdock::OnAlt()
{
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
}
void AMurdock::OffAlt()
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AMurdock::OnShield()
{
	if (BehaviorState == MurdockBehaviorState::EIdle && !ShieldSkill->GetIsCoolTiming())
	{
		OnStartActionMontage(MurdockBehaviorState::EShield);
	}
}
void AMurdock::OffShield()
{
	if (BehaviorState == MurdockBehaviorState::EShield)
	{
		OnEndActionMontage();
	}
}


void AMurdock::OnSpreadShot()
{
	if (BehaviorState == MurdockBehaviorState::EIdle && !SpreadShotSkill->GetIsCoolTiming() )
	{
		//Clog::Log(SpreadShotSkill);
		OnStartActionMontage(MurdockBehaviorState::ESpreadShot);
	}
}
void AMurdock::OffSpreadShot()
{
	if (BehaviorState == MurdockBehaviorState::ESpreadShot)
	{
		OnEndActionMontage();
	}
}

void AMurdock::OnUltimate()
{

	if (BehaviorState == MurdockBehaviorState::EIdle && !UltimateSkill->GetIsCoolTiming())
	{
		//Clog::Log(int(BehaviorState));
		FVector FrontVector = UKismetMathLibrary::Cross_VectorVector(PlayerMainCamera->GetRightVector(), GetActorUpVector());
		SetActorRotation(FrontVector.Rotation().Quaternion());
		GetCharacterMovement()->bOrientRotationToMovement = false;
		FrontYaw = GetBaseAimRotation().Yaw;
		OnStartActionMontage(MurdockBehaviorState::EUltimate);
		Stop();
	}
	else if (BehaviorState == MurdockBehaviorState::EUltimate)
	{
		OnEndActionMontage(MurdockBehaviorState::EUltimate);
		//SolveStop();
	}
}

void AMurdock::CameraLag(bool Active, float CameraLagSpeed)
{
	SpringArm->bEnableCameraLag = Active;
	SpringArm->CameraLagSpeed = CameraLagSpeed;
}

void AMurdock::MoveCamera(FVector socketOffset, FVector tragetOffset,FVector location, FRotator rotation, float targetArmLength)
{
	SpringArm->SetRelativeLocation(location);

	SpringArm->SetRelativeRotation(rotation);

	SpringArm->TargetArmLength = targetArmLength;

	SpringArm->SocketOffset = socketOffset;

	SpringArm->TargetOffset = tragetOffset;
}

void AMurdock::MoveCamera(FName targetName)
{
	

	FTransform Muzzle_3_transform = GetMesh()->GetSocketTransform(targetName, ERelativeTransformSpace::RTS_Actor);
	FVector Muzzle_3_vector;

	Muzzle_3_transform.TransformVector(Muzzle_3_vector);



	//SpringArm->SetRelativeLocation(FVector(0, 0, 60));
	SpringArm->TargetArmLength = 0.0f;
	//SpringArm->bDoCollisionTest = false;
	//SpringArm->bUsePawnControlRotation = true;

	SpringArm->SocketOffset = FVector(0, 0, 0);


}

template<typename T>
void AMurdock::StartCameraFOV(float IncreaseFOV,float DuringTime, T* OtherClass, const FName FunctionName)
{
	StartCameraFOV(IncreaseFOV, DuringTime);

	EndZoomFunc.BindUFunction(OtherClass, FunctionName);

}

void AMurdock::StartCameraFOV(float IncreaseFOV, float DuringTime)
{
	increaseFOV = IncreaseFOV;
	currentFOV = PlayerMainCamera->FieldOfView;
	isChangeFOV = true;
	duringTime = DuringTime;
	reverseDuringTime = 1.0f / DuringTime;
	ZoomTime = 0.0f;
	EndZoomFunc.Clear();
}

void AMurdock::DoZoom(float DeltaTime)
{
	if (ZoomTime <= duringTime)
	{
		
		float Fov = currentFOV + ZoomCurveFloat->GetFloatValue(ZoomTime * reverseDuringTime) * increaseFOV;
		PlayerMainCamera->FieldOfView = Fov;
		ZoomTime += DeltaTime;
	}
	else
	{
		PlayerMainCamera->FieldOfView = currentFOV + increaseFOV;
		isChangeFOV = false;
		if (EndZoomFunc.IsBound())
		{
			EndZoomFunc.Execute();
		}
	}

}

UCActionComponent* AMurdock::GetActionComponent()
{
	switch(BehaviorState)
	{
		case MurdockBehaviorState::EIdle:
		{
			return MurdockWeapon;
		}
		break;
		case MurdockBehaviorState::EFire:
		{
			return MurdockWeapon;
		}
		break;
		case MurdockBehaviorState::EShield:
		{
			return ShieldSkill;
		}
		break;
		case MurdockBehaviorState::ESpreadShot:
		{
			return SpreadShotSkill;
		}
		break;
		case MurdockBehaviorState::EUltimate:
		{
			return UltimateSkill;
		}
		break;
		case MurdockBehaviorState::EMAX:
		{
			return nullptr;
		}
		break;
	}


	return nullptr;
}

void AMurdock::OnStartActionMontage(MurdockBehaviorState ActionEnum)
{
	if (BehaviorState != ActionEnum)
		BehaviorState = ActionEnum;

	UCActionComponent* action = GetActionComponent();
	CheckNull(action);
	action->OnStartAction();
}

void AMurdock::OnEndActionMontage(MurdockBehaviorState ActionEnum)
{
	UCActionComponent* action = GetActionComponent();
	Clog::Log(action);
	CheckNull(action);
	action->OnEndAction();
	BehaviorState = ActionEnum;
}

void AMurdock::BeginNotifyAction()
{
	UCActionComponent* actionComp = GetActionComponent();
	CheckNull(actionComp);
	actionComp->BeginNotifyAction();
}

void AMurdock::MiddleNotifyAction()
{
	UCActionComponent* actionComp = GetActionComponent();
	CheckNull(actionComp);
	actionComp->MiddleNotifyAction();
}

void AMurdock::EndNotifyAction()
{
	UCActionComponent* actionComp = GetActionComponent();
	CheckNull(actionComp);
	actionComp->EndNotifyAction();
}
