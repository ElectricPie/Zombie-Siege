// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/Widgets/PlayerMoneyWidget.h"

#include "Components/TextBlock.h"

void UPlayerMoneyWidget::SetMoneyText(const int32 MoneyAmount) const
{
	const FText Text = FText::FromString(FString::Printf(TEXT("%s %d"), *TextPrefix, MoneyAmount));
	MoneyText->SetText(Text);
}
