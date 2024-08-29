// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/Widgets/MenuButton.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UMenuButton::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	TextBlock->SetText(Text);
}

void UMenuButton::NativeConstruct()
{
	Super::NativeConstruct();

	Button->OnClicked.AddDynamic(this, &UMenuButton::OnButtonClicked);
}

void UMenuButton::OnButtonClicked()
{
	OnClicked.Broadcast();
}
