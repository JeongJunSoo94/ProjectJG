#include "BaseSystem/InGameModeBase.h"
#include "Global.h"
#include "BaseSystem/GameHUD.h"
#include "BaseSystem/projectJG_GameInstance.h"
#include "GameFramework/Pawn.h"
#include "DataAssets/CharacterDataAsset.h"
#include "Widgets/Title/UserWidget_MainTitle.h"
#include "BaseSystem/ObjectPoolFactory.h"

AInGameModeBase::AInGameModeBase()
{
	HUDClass = AGameHUD::StaticClass();
	//TSubclassOf<APawn> PawnClass;
	//CHelpers::GetClass<APawn>(&PawnClass, "Blueprint'/Game/Developers/USER/Character/BP_LtBelica.BP_LtBelica_C'");
	//DefaultPawnClass = PawnClass;

	CHelpers::GetAsset<UCharacterDataAsset>(&characterDataAsset, "CharacterDataAsset'/Game/Developers/GohyeongJu/DataAsset/DB_CharacterDataAsset.DB_CharacterDataAsset'");
	
	//PlayerControllerClass = AController_StartMenu::StaticClass();
	CHelpers::CreateActorComponent<UObjectPoolFactory>(this, &ObjectPoolFactory, "ObjectPoolFactory");
}

void AInGameModeBase::BeginPlay()
{



	Super::BeginPlay();
	
	
		

	//Test.SetNum(256);

	


	//AActor* T = nullptr;
	//Test[1].Enqueue(T);

	//Clog::Log(Test[1].IsEmpty());
}

void AInGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	UprojectJG_GameInstance* gameInstance = Cast<UprojectJG_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	switch (gameInstance->GetData().SelectCharacter)
	{
	case ECharacterSelect::EBelica:
	{
		DefaultPawnClass = characterDataAsset->CharacterDatas[0].Pawn;
		Clog::Log("Belica");
	}
	break;
	case ECharacterSelect::EMurdock:
	{
		DefaultPawnClass = characterDataAsset->CharacterDatas[1].Pawn;
		Clog::Log("Murdock");
	}
	break;
	default:
	{
		DefaultPawnClass = characterDataAsset->CharacterDatas[0].Pawn;
		
	}
	break;
	}

	Super::InitGame(MapName, Options, ErrorMessage);
}


