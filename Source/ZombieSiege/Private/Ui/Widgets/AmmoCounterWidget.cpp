// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/Widgets/AmmoCounterWidget.h"

#include "Components/TextBlock.h"

void UAmmoCounterWidget::UpdateAmmoText(const int32 CurrentAmmo, const int32 MaxAmmo)
{
	const FText AmmoText = FText::FromString(FString::Printf(TEXT("%d / %d"), CurrentAmmo, MaxAmmo));
	AmmoTextBlock->SetText(AmmoText);
}
