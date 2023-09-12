// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Title/UserWidget_MainTitle.h"
#include "Global.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"

void UUserWidget_MainTitle::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	MainTitle_VerticalBox = Cast<UVerticalBox>(GetWidgetFromName(TEXT("MainButtonBox")));
}

void UUserWidget_MainTitle::GetOutCurrentPanel()
{
	//MainTitle_VerticalBox->RenderTransform.Translation
}