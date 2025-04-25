// Fill out your copyright notice in the Description page of Project Settings.

#include "Ui/Widgets/OptionsWidget.h"

#include "Ui/Widgets/MenuButton.h"

void UOptionsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CloseButton->OnClicked.AddDynamic(this, &UOptionsWidget::OnCloseButtonClicked);
}

void UOptionsWidget::OnCloseButtonClicked()
{
	OnOptionsClosedEvent.Broadcast();
}
