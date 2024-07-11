// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/Widgets/GameHudWidget.h"

#include "AmmoCounterWidget.h"
#include "Components/TextBlock.h"
#include "Player/PlayerCharacter.h"
#include "Weapons/Gun.h"


void UGameHudWidget::Setup(APlayerCharacter* NewPlayerCharacter)
{
	if (NewPlayerCharacter)
	{
		NewPlayerCharacter->OnWeaponChangedEvent.AddUniqueDynamic(this, &UGameHudWidget::OnWeaponChanged);
	}
}

void UGameHudWidget::UpdateInteractText(FText const & InteractText)
{
	InteractTextBlock->SetText(InteractText);
}

void UGameHudWidget::ShowInteractText(bool bShowInteractText)
{
	if (bShowInteractText)
	{
		UE_LOG(LogTemp, Warning, TEXT("Showing"));
		InteractTextBlock->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		InteractTextBlock->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGameHudWidget::OnAmmoChanged(int32 NewAmmoCount, int32 MaxAmmo)
{
	UE_LOG(LogTemp, Warning, TEXT("Ammo Updated"));
	AmmoCounterWidget->UpdateAmmoText(NewAmmoCount, MaxAmmo);
}

void UGameHudWidget::OnWeaponChanged(AGun* NewWeapon)
{
	if (NewWeapon == nullptr) return;
	
	if (CurrentWeapon.IsValid() && AmmoChangeHandle.IsValid())
	{
		CurrentWeapon->OnAmmoChangedEvent.Remove(AmmoChangeHandle);
	}

	
	CurrentWeapon = NewWeapon;
	AmmoCounterWidget->UpdateAmmoText(CurrentWeapon->GetCurrentAmmo(), CurrentWeapon->GetMaxAmmo());
	AmmoChangeHandle = NewWeapon->OnAmmoChangedEvent.AddUObject(this, &UGameHudWidget::OnAmmoChanged);
}
