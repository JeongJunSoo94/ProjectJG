#include "Character/Enemies/MeleeEnemy/MeleeEnemyCharacter.h"
#include "Global.h"
#include "Character/Enemies/MeleeEnemy/MeleeEnemyAIController.h"

AMeleeEnemyCharacter::AMeleeEnemyCharacter()
{
	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/ParagonMinions/Characters/Minions/White_Camp_Minion/Meshes/Minion_melee.Minion_melee'");
	GetMesh()->SetSkeletalMesh(mesh);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	TSubclassOf<AMeleeEnemyAIController> aicontroller;
	CHelpers::GetClass<AMeleeEnemyAIController>(&aicontroller, "Blueprint'/Game/Developers/JJS/Enemy/BP_MeleeEnemyAIController.BP_MeleeEnemyAIController_C'");
	AIControllerClass = aicontroller;

}

void AMeleeEnemyCharacter::BeginPlay()
{
	SetHealthWidgetSizeAndLocation(FVector(0, 0, 150), FVector2D(120, 20));
	Super::BeginPlay();
}

void AMeleeEnemyCharacter::Tick(float DeltaTime)
{
}
