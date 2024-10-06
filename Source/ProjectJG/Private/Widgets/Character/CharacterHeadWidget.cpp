#include "Widgets/Character/CharacterHeadWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"

void UCharacterHeadWidget::SetDisplayText(FString TextToDisplay)
{
	if (DisplayText)
	{
		DisplayText->SetText(FText::FromString(TextToDisplay));
	}
}

void UCharacterHeadWidget::ShowPlayerNetRole(APawn* InPawn)
{
	Owner = InPawn;
	const ENetRole LocalRole = InPawn->GetLocalRole();
	const ENetRole RemoteRole = InPawn->GetRemoteRole();
	switch (LocalRole)
	{
	case ENetRole::ROLE_Authority:
		LocalRoleStr = FString("Authority");
		break;
	case ENetRole::ROLE_AutonomousProxy:
		LocalRoleStr = FString("Autonomous Proxy");
		break;
	case ENetRole::ROLE_SimulatedProxy:
		LocalRoleStr = FString("Simulated Proxy");
		break;
	case ENetRole::ROLE_None:
		LocalRoleStr = FString("None");
		break;
	}
	switch (RemoteRole)
	{
	case ENetRole::ROLE_Authority:
		RemoteRoleStr = FString("Authority");
		break;
	case ENetRole::ROLE_AutonomousProxy:
		RemoteRoleStr = FString("Autonomous Proxy");
		break;
	case ENetRole::ROLE_SimulatedProxy:
		RemoteRoleStr = FString("Simulated Proxy");
		break;
	case ENetRole::ROLE_None:
		RemoteRoleStr = FString("None");
		break;
	}
	//FString RemoteRoleString = FString::Printf(TEXT("Remote Role: %s"), *RemoteRoleStr);
	//SetDisplayText(RemoteRoleString);
	//SetPlayerText();
}

void UCharacterHeadWidget::SetPlayerText()
{
	APlayerState* player = Cast<APlayerState>(Owner->GetPlayerState());
	if(player)
	{
		FString ConnectionStr = Owner->GetNetConnection() != nullptr ? TEXT("Vaild Connecttion") : TEXT("Invaild Connecttion");
		if (DisplayText)
		{
			FString InfoString;
			
			if (Owner->GetController() != nullptr)
			{
				InfoString = FString::Printf(TEXT("PlayerName: %s \nLocal Role: %s \nRemote Role: %s\n Connection: %s \n Have Controller"), *(player->GetPlayerName()), *LocalRoleStr, *RemoteRoleStr,*ConnectionStr);
				DisplayText->SetText(FText::FromString(InfoString));
			}
			else
			{
				InfoString = FString::Printf(TEXT("PlayerName: %s \nLocal Role: %s \nRemote Role: %s\n Connection: %s\n Haven't Controller"), *(player->GetPlayerName()), *LocalRoleStr, *RemoteRoleStr, *ConnectionStr);
				DisplayText->SetText(FText::FromString(InfoString));
			}
			//DisplayText->SetText(FText::FromString(player->GetPlayerName()));
		}
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(NameMatchTimer, this, &UCharacterHeadWidget::SetPlayerText, 1.0f, false);
	}
}

void UCharacterHeadWidget::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	RemoveFromParent();
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}

