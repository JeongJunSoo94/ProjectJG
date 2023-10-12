#include "Widgets/Title/UserWidget_MainTitle.h"
#include "Global.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Components/ComboBoxKey.h"

#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameStateBase.h"
#include "BaseSystem/projectJG_GameInstance.h"
#include "BaseSystem/StartGameModeBase.h"

UUserWidget_MainTitle::UUserWidget_MainTitle(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//Super::NativeOnInitialized();
	//외부 데이터에서 초기화 되었다는 가정을 위해 여기서 만들어줌.
	StageFNameData.Add(StageData::Stage0);
	StageFNameData.Add(StageData::Stage1);
	StageFNameData.Add(StageData::Stage2);
	/*for (FName Name : StageFNameData)
	{
		StageComboBox_Key->AddOption(Name);
	}*/
}
void UUserWidget_MainTitle::SelectStage(FName StageNumber)
{

	int32 StageIntData = StageFNameData.Find(StageNumber);
	
	GameData.StageNumber = StaticCast<EStageNumber>(StageIntData);
	
}

void UUserWidget_MainTitle::SelectDifficulty(FName Difficulty)
{
	using namespace Difficulty;
	if (Easy == Difficulty)
	{
		GameData.Difficulty = EDifficulty::EEasy;
	}
	else if (Normal == Difficulty)
	{
		GameData.Difficulty = EDifficulty::ENormal;
	}
	else if (Hard == Difficulty)
	{
		GameData.Difficulty = EDifficulty::EHard;
	}
	else
	{
		GameData.Difficulty = EDifficulty::EEasy;
	}
}

void UUserWidget_MainTitle::SelectCharacter(ECharacterSelect SelectCharacterEnum)
{
	GameData.SelectCharacter = SelectCharacterEnum;
}

void UUserWidget_MainTitle::PrintStageNumber()
{
	Clog::Log("Stage%d", StaticCast<int32>(GameData.StageNumber));
}

void UUserWidget_MainTitle::PrintDifficulty()
{
	//Clog::Log(GetWorld()->GetAuthGameMode()->GetName());
	Clog::Log(GetWorld()->GetGameState()->GetName());
	Clog::Log("Difficulty%d", StaticCast<int32>(GameData.Difficulty));
}


void UUserWidget_MainTitle::PrintCharacterEnum()
{
	//UGameplayStatics::GetGameInstance(GetWorld());
	Clog::Log("Character%d", StaticCast<int32>(GameData.SelectCharacter));
}

void UUserWidget_MainTitle::OnStartButton(FName StageName)
{
	UprojectJG_GameInstance* gameInstance = Cast<UprojectJG_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	gameInstance->SetGameData(GameData);


	UGameplayStatics::OpenLevel(GetWorld(), StageName);
}

void UUserWidget_MainTitle::SelectCharacterWorldSpawn(ECharacterSelect SelectCharacterEnum)
{
	if(StartGameModeBase==nullptr)
		StartGameModeBase = Cast<AStartGameModeBase>(GetWorld()->GetAuthGameMode());
	switch(SelectCharacterEnum)
	{
		case ECharacterSelect::EBelica:
		{
			StartGameModeBase->SpawnSelectActor(0);
		}
		break;
		case ECharacterSelect::EMurdock:
		{
			StartGameModeBase->SpawnSelectActor(1);
		}
		break;
		case ECharacterSelect::EGadget:
		{
			StartGameModeBase->SpawnSelectActor(2);
		}
		break;
		case ECharacterSelect::EKallari:
		{
			StartGameModeBase->SpawnSelectActor(3);
		}
		break;
	}
}