#include "BaseSystem/StartGameModeBase.h"
#include "Global.h"
#include "BaseSystem/StartGameHUD.h"
#include "DataAssets/SelectCharacterDataAsset.h"
#include "Camera/CameraActor.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"

AStartGameModeBase::AStartGameModeBase()
{
	//HUDClass = AStartGameHUD::StaticClass();
	DefaultPawnClass = nullptr;
	//PlayerControllerClass = AController_StartMenu::StaticClass();
	CHelpers::GetAsset<USelectCharacterDataAsset>(&SelectCharacterDataAsset, "SelectCharacterDataAsset'/Game/Developers/JJS/CharacterUI/DA_SelectCharacter.DA_SelectCharacter'");
	CHelpers::GetAsset<USoundCue>(&BGM, "SoundCue'/Game/Developers/JJS/BGM/LobbyBGM.LobbyBGM'");
}

void AStartGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	TArray<AActor*> arrOutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), arrOutActors);

	Camera = arrOutActors[0];
	Clog::Log(Camera);
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AActor::StaticClass(),"SpawnActor", arrOutActors);
	SpawnActor = arrOutActors[0];
	Clog::Log(SpawnActor);
	for (auto characterInfo : SelectCharacterDataAsset->SelectCharacters)
	{
		AActor* actor = GetWorld()->SpawnActor<AActor>(characterInfo.SelectCharacter, SpawnActor->GetActorLocation(), SpawnActor->GetActorRotation());
		SelectCharacters.Add(actor);
		SetActive(actor, false);
	}
}

void AStartGameModeBase::SpawnSelectActor(int indexNumber)
{
	SetActive(SelectCharacters[curIndexNum], false);
	curIndexNum = indexNumber;
	SetActive(SelectCharacters[curIndexNum], true);
}

void AStartGameModeBase::SetActive(AActor* actor, bool value)
{
	actor->SetActorHiddenInGame(!value);
	actor->SetActorEnableCollision(value);
}

void AStartGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	AudioComponent =UGameplayStatics::SpawnSound2D(GetWorld(), BGM, 1.f, 1.f, 0.f, nullptr, true, true);
}