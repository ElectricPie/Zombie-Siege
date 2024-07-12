// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/Widgets/GameHudWidget.h"

#include "AmmoCounterWidget.h"
#include "PlayerMoneyWidget.h"
#include "Components/MoneyStoreComponent.h"
#include "Components/TextBlock.h"
#include "Player/PlayerCharacter.h"
#include "Player/TopDownPlayerController.h"
#include "Weapons/Gun.h"


void UGameHudWidget::Setup(ATopDownPlayerController* PlayerController, APlayerCharacter* PlayerCharacter)
{
	if (PlayerCharacter)
	{
		PlayerCharacter->OnWeaponChangedEvent.AddUniqueDynamic(this, &UGameHudWidget::OnWeaponChanged);
	}

	if (PlayerController)
	{
		if (UMoneyStoreComponent* MoneyStore = PlayerController->GetComponentByClass<UMoneyStoreComponent>())
		{
			MoneyStore->OnMoneyChangedEvent.AddUObject(this, &UGameHudWidget::OnMoneyChanged);
			MoneyWidget->SetMoneyText(MoneyStore->GetMoney());
		}
		
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
		InteractTextBlock->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		InteractTextBlock->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGameHudWidget::OnAmmoChanged(int32 NewAmmoCount, int32 MaxAmmo)
{
	AmmoCounterWidget->UpdateAmmoText(NewAmmoCount, MaxAmmo);
}

void UGameHudWidget::OnWeaponChanged(AGun* NewWeapon)
{
	if (NewWeapon == nullptr) return;
	
	if (CurrentWeapon.IsValid())
	{
		if (AmmoChangeHandle.IsValid())
		{
			CurrentWeapon->OnAmmoChangedEvent.Remove(AmmoChangeHandle);
		}
		if (WeaponReloadHandle.IsValid())
		{
			CurrentWeapon->OnReloadStateChangedEvent.Remove(WeaponReloadHandle);
		}
	}
	
	CurrentWeapon = NewWeapon;
	AmmoCounterWidget->UpdateAmmoText(CurrentWeapon->GetCurrentAmmo(), CurrentWeapon->GetMaxAmmo());
	AmmoChangeHandle = NewWeapon->OnAmmoChangedEvent.AddUObject(this, &UGameHudWidget::OnAmmoChanged);
	WeaponReloadHandle = NewWeapon->OnReloadStateChangedEvent.AddUObject(this, &UGameHudWidget::OnWeaponReloadStateChanged);
}

void UGameHudWidget::OnWeaponReloadStateChanged(bool bIsReloading)
{
	AmmoCounterWidget->ShowReloadingMessage(bIsReloading);
}

void UGameHudWidget::OnMoneyChanged(int32 NewMoneyAmount, int32 AmountChanged)
{
	MoneyWidget->SetMoneyText(NewMoneyAmount);
}