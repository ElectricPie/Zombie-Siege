// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/Widgets/GameHudWidget.h"

#include "Components/TextBlock.h"

void UGameHudWidget::UpdateInteractText(FText const & InteractText)
{
	InteractTextBlock->SetText(InteractText);
}