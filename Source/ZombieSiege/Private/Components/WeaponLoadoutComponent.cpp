// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WeaponLoadoutComponent.h"

#include "Weapons/Gun.h"

// Sets default values for this component's properties
UWeaponLoadoutComponent::UWeaponLoadoutComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UWeaponLoadoutComponent::AddWeapon(AGun* Weapon, bool bEquip /*= false*/)
{
	if (Weapon == nullptr || Weapons.Contains(Weapon)) return;

	const int32 Index = Weapons.Add(Weapon);
	if (bEquip || Weapons.IsEmpty())
	{
		EquippedWeaponIndex = Index;
		OnWeaponChangedEvent.Broadcast(Weapon);
	}
	Weapon->SetVisibility(bEquip);;
	OnWeaponAddedEvent.Broadcast(Weapon);
}

AGun* UWeaponLoadoutComponent::GetEquippedWeapon()
{
	if (EquippedWeaponIndex >= Weapons.Num()) return nullptr;

	return Weapons[EquippedWeaponIndex].Get();
}

void UWeaponLoadoutComponent::EquipNextWeapon()
{
	// No need to change weapons if there is only one
	if (Weapons.Num() <= 1) return; 
	
	// Hide the current weapon
	if (AGun* EquippedWeapon = GetEquippedWeapon())
	{
		// Prevent changing weapons while reloading
		if (EquippedWeapon->GetIsReloading()) return;
		
		EquippedWeapon->SetVisibility(false);
	}
	
	if (EquippedWeaponIndex + 1 >= Weapons.Num())
	{
		EquippedWeaponIndex = 0;
	}
	else
	{
		EquippedWeaponIndex++;
	}

	// Show the new weapon
	if (AGun* EquippedWeapon = GetEquippedWeapon())
	{
		OnWeaponChangedEvent.Broadcast(EquippedWeapon);
		EquippedWeapon->SetVisibility(true);
	}
}

void UWeaponLoadoutComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (AGun* EquippedWeapon = GetEquippedWeapon())
	{
		OnWeaponChangedEvent.Broadcast(EquippedWeapon);
	}
}
