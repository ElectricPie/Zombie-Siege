// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/Widgets/MoneyChangedWidget.h"

#include "Components/TextBlock.h"

void UMoneyChangedWidget::SetAmount(const int32 Amount)
{
	FString Prefix = TEXT("");
	FColor TextColor = PositiveColor;
	if (Amount < 0)
	{
		TextColor = NegativeColor;
	}
	else
	{
		Prefix = TEXT("+");
	}
	
	MoneyTextBlock->SetColorAndOpacity(TextColor);
	const FText Text = FText::FromString(FString::Printf(TEXT("%s%d"), *Prefix, Amount));
	MoneyTextBlock->SetText(Text);

	TriggerAnimation();
}
