// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieSiege/Public/Ui/WidgetControllers/AmmoCounterWidgetController.h"

#include "Components/WeaponLoadoutComponent.h"
#include "Player/PlayerCharacter.h"
#include "Weapons/WeaponStatsDataAsset.h"

void UAmmoCounterWidgetController::BindCallbackToDependencies()
{
	if (const APlayerCharacter* PlayerCharacter = PlayerController->GetPawn<APlayerCharacter>())
	{
		UWeaponLoadoutComponent* WeaponLoadoutComponent = PlayerCharacter->GetWeaponLoadoutComponent();
		WeaponLoadoutComponent->OnWeaponChangedEvent.AddDynamic(this, &UAmmoCounterWidgetController::OnWeaponChanged);
	}	
}

void UAmmoCounterWidgetController::BroadcastInitialValues()
{
	if (const APlayerCharacter* PlayerCharacter = PlayerController->GetPawn<APlayerCharacter>())
	{
		UWeaponLoadoutComponent* WeaponLoadoutComponent = PlayerCharacter->GetWeaponLoadoutComponent();
		if (const AGun* EquippedWeapon = WeaponLoadoutComponent->GetEquippedWeapon())
		{
			AmmoChangedEvent.Broadcast(EquippedWeapon->GetCurrentAmmo(), EquippedWeapon->GetWeaponStats()->GetMaxAmmo());
		}
		else
		{
			AmmoChangedEvent.Broadcast(0, 0);
		}
	}	
}

void UAmmoCounterWidgetController::OnWeaponChanged(AGun* NewWeapon)
{
	if (CurrentWeapon)
	{
		CurrentWeapon->OnAmmoChangedEvent.Remove(AmmoChangeHandle);
		CurrentWeapon->OnReloadStateChangedEvent.Remove(WeaponReloadHandle);
	}

	if (NewWeapon)
	{
		WeaponReloadHandle = NewWeapon->OnReloadStateChangedEvent.AddLambda([this](const bool bIsReloading)
	   {
		   ReloadStateChangedEvent.Broadcast(bIsReloading);
	   });
		AmmoChangeHandle = NewWeapon->OnAmmoChangedEvent.AddLambda([this](const int32 CurrentAmmo, const int32 MaxAmmo)
		{
			AmmoChangedEvent.Broadcast(CurrentAmmo, MaxAmmo);
		});
		AmmoChangedEvent.Broadcast(NewWeapon->GetCurrentAmmo(), NewWeapon->GetWeaponStats()->GetMaxAmmo());
	}

	CurrentWeapon = NewWeapon;
}
