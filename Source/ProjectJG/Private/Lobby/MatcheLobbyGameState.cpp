#include "Lobby/MatcheLobbyGameState.h"
#include "MultiplayerSessionsSubsystem.h"
#include "Lobby/MatchePlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Lobby/MatcheGameMode.h"
#include "Net/UnrealNetwork.h"

void AMatcheLobbyGameState::BeginPlay()
{

}

void AMatcheLobbyGameState::UpdatePlayerControllers()
{
	for (auto Controller : PlayerControllers)
	{
		AMatchePlayerController* MatcheController = Cast<AMatchePlayerController>(Controller);
		if(MatcheController)
			MatcheController->UpdateMatcheWidget();
	}
}

void AMatcheLobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMatcheLobbyGameState, NumPublicConnections);
}

void AMatcheLobbyGameState::SetNumPublicConnections_Implementation()
{
	AMatcheGameMode* GameMode = Cast<AMatcheGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
		NumPublicConnections = GameMode->GetSessionPlayerNum();
}