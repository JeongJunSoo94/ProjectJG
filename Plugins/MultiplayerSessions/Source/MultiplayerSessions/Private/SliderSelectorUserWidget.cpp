#include "SliderSelectorUserWidget.h"
#include "Engine.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"


void USliderSelectorUserWidget::UpdateText()
{
	if (Options.IsEmpty())
		return;
	if (SelectText)
	{
		SelectText->SetText(FText::FromString(Options[CurIdx].ToString()));
		SelectDelegate.Broadcast(Options[CurIdx].ToString());
	}
}

void USliderSelectorUserWidget::OnNext()
{
	if (Options.Num()<= CurIdx+1)
		CurIdx = 0;
	else
		++CurIdx;
	UpdateText();
}

void USliderSelectorUserWidget::OnPrevious()
{
	if (CurIdx - 1 < 0)
		CurIdx = Options.Num()-1;
	else
		--CurIdx;
	UpdateText();
}

void USliderSelectorUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Options.Num() >= 0)
	{
		if (LeftArrowButton)
		{
			LeftArrowButton->OnClicked.AddDynamic(this, &ThisClass::OnPrevious);
		}
		if (RightArrowButton)
		{
			RightArrowButton->OnClicked.AddDynamic(this, &ThisClass::OnNext);
		}
		CurIdx = 0;
		UpdateText();
	}
}
