// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/Widgets/AmmoCounterWidget.h"

#include "Components/TextBlock.h"

void UAmmoCounterWidget::UpdateAmmoText(const int32 CurrentAmmo, const int32 MaxAmmo)
{
	const FText AmmoText = FText::FromString(FString::Printf(TEXT("%d / %d"), CurrentAmmo, MaxAmmo));
	AmmoTextBlock->SetText(AmmoText);
}

void UAmmoCounterWidget::ShowReloadingMessage(const bool bShowReloadingMessage = true)
{
	if (bShowReloadingMessage)
	{
		AmmoTextBlock->SetVisibility(ESlateVisibility::Collapsed);
		ReloadingTextBlock->SetVisibility(ESlateVisibility::Visible);
		return;
	}
	
	AmmoTextBlock->SetVisibility(ESlateVisibility::Visible);
	ReloadingTextBlock->SetVisibility(ESlateVisibility::Collapsed);
}

