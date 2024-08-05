#include "HomeMenuUserWidget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MenuUserWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"

void UHomeMenuUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &ThisClass::StartButtonClicked);
	}
	if (MultiButton)
	{
		MultiButton->OnClicked.AddDynamic(this, &ThisClass::MultiButtonClicked);
	}
	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &ThisClass::QuitButtonClicked);
	}
}

void UHomeMenuUserWidget::StartButtonClicked()
{
	if (MenuUser)
	{
		UWorld* World = GetWorld();
		UGameplayStatics::OpenLevel(World, "/Game/Developers/JJS/TestMap/TestMap?listen", true);
	}
}

void UHomeMenuUserWidget::MultiButtonClicked()
{
	if (MenuUser)
	{
		MenuUser->WidgetSwitcher->SetActiveWidgetIndex(1);
		MenuUser->UndoButton->SetVisibility(ESlateVisibility::Visible);
	}
}

void UHomeMenuUserWidget::QuitButtonClicked()
{
	UWorld* World = GetWorld();
	if (World)
	{
		UKismetSystemLibrary::QuitGame(World, nullptr, EQuitPreference::Quit, false);
	}
}
