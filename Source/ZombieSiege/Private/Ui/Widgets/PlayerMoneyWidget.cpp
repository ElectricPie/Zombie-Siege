// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/Widgets/PlayerMoneyWidget.h"

#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/TextBlock.h"
#include "Ui/Widgets/MoneyChangedWidget.h"

void UPlayerMoneyWidget::SetMoneyText(const int32 MoneyAmount, const int32 AmountChanged)
{
	const FText Text = FText::FromString(FString::Printf(TEXT("%s %d"), *TextPrefix, MoneyAmount));
	MoneyText->SetText(Text);
	
	if (MoneyChangedWidgetClass && AmountChanged != 0)
	{
		// TODO: Convert to using pool instead of creating
		UMoneyChangedWidget* MoneyChangedWidget = CreateWidget<UMoneyChangedWidget>(this, MoneyChangedWidgetClass);
		if (UOverlaySlot* MoneyChangedSlot = Cast<UOverlaySlot>(Overlay->AddChild(MoneyChangedWidget)))
		{
			const FVector2D Size = GetDesiredSize();
			MoneyChangedSlot->SetPadding(FMargin(Size.X / 2, 0.f, 0.f, 0.f));
			MoneyChangedWidget->SetAmount(AmountChanged);
		}
		else
		{
			MoneyChangedWidget->RemoveFromParent();
			MoneyChangedWidget->MarkAsGarbage();
		}
	}
}
