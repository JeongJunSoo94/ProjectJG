#include "PauseMenuUserWidget.h"
#include "Components/Button.h"
#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine.h"

void UPauseMenuUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ContinueButton)
	{
		ContinueButton->OnClicked.AddDynamic(this, &ThisClass::ContinueButtonClicked);
	}

	if (OptionButton)
	{
		OptionButton->OnClicked.AddDynamic(this, &ThisClass::OptionButtonClicked);
	}

	if (LeaveButton)
	{
		LeaveButton->OnClicked.AddDynamic(this, &ThisClass::LeaveButtonClicked);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &ThisClass::QuitButtonClicked);
	}

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}
}

void UPauseMenuUserWidget::ContinueButtonClicked()
{
	UWorld* World = GetWorld();
	if (World)
	{
		PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false);
		}
	}
	RemoveFromViewport();
}

void UPauseMenuUserWidget::OptionButtonClicked()
{
}

void UPauseMenuUserWidget::LeaveButtonClicked()
{
	UWorld* World = GetWorld();
	if (World)
	{
		UGameplayStatics::OpenLevel(World, *PathToMainMenu, true);
		MultiplayerSessionsSubsystem->DestroySession();
	}
}

void UPauseMenuUserWidget::QuitButtonClicked()
{
	UWorld* World = GetWorld();
	if (World)
	{
		UKismetSystemLibrary::QuitGame(World, nullptr, EQuitPreference::Quit, false);
	}
}
