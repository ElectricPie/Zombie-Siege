// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/Widgets/GameHudWidget.h"

#include "AmmoCounterWidget.h"
#include "PlayerMoneyWidget.h"
#include "Components/MoneyStoreComponent.h"
#include "Components/TextBlock.h"
#include "Components/WeaponLoadoutComponent.h"
#include "Player/PlayerCharacter.h"
#include "Player/TopDownPlayerController.h"
#include "Weapons/Gun.h"
#include "ZombieSiege/Public/Weapons/WeaponStatsDataAsset.h"


void UGameHudWidget::Setup(ATopDownPlayerController* PlayerController, APlayerCharacter* PlayerCharacter)
{
	if (PlayerCharacter)
	{
		if (UWeaponLoadoutComponent* WeaponLoadout = PlayerCharacter->GetComponentByClass<UWeaponLoadoutComponent>())
		{
			WeaponLoadout->OnWeaponChangedEvent.AddUniqueDynamic(this, &UGameHudWidget::OnWeaponChanged);
		}
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

void UGameHudWidget::UpdateInteractText(FText const& InteractText)
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

void UGameHudWidget::UpdateRoundNumber(int32 RoundNumber)
{
	RoundText->SetText(FText::FromString(FString::FromInt(RoundNumber)));
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
	AmmoCounterWidget->UpdateAmmoText(CurrentWeapon->GetCurrentAmmo(), CurrentWeapon->GetWeaponStats()->GetMaxAmmo());
	AmmoChangeHandle = NewWeapon->OnAmmoChangedEvent.AddUObject(this, &UGameHudWidget::OnAmmoChanged);
	WeaponReloadHandle = NewWeapon->OnReloadStateChangedEvent.AddUObject(
		this, &UGameHudWidget::OnWeaponReloadStateChanged);
}

void UGameHudWidget::OnWeaponReloadStateChanged(bool bIsReloading)
{
	AmmoCounterWidget->ShowReloadingMessage(bIsReloading);
}

void UGameHudWidget::OnMoneyChanged(const int32 NewMoneyAmount, const int32 AmountChanged)
{
	MoneyWidget->SetMoneyText(NewMoneyAmount, AmountChanged);
}
