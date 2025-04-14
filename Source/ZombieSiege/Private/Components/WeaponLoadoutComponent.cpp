// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WeaponLoadoutComponent.h"

#include "Net/UnrealNetwork.h"
#include "Weapons/Gun.h"

// Sets default values for this component's properties
UWeaponLoadoutComponent::UWeaponLoadoutComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UWeaponLoadoutComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UWeaponLoadoutComponent, EquippedWeaponIndex);
	DOREPLIFETIME(UWeaponLoadoutComponent, Weapons);
}

void UWeaponLoadoutComponent::AddWeapon_Server(AGun* NewWeapon, bool bEquip /*= false*/)
{
	check(GetOwner()->HasAuthority());
	if (NewWeapon == nullptr || Weapons.Contains(NewWeapon))
		return;

	NewWeapon->SetOwner(GetOwner());
	
	// Force equip if there are no weapons
	if (Weapons.IsEmpty())
	{
		bEquip = true;
	}

	if (AGun* EquippedWeapon = GetEquippedWeapon())
	{
		EquippedWeapon->CancelReload();
	}

	const int32 NewWeaponIndex = Weapons.Add(NewWeapon);
	if (bEquip)
	{
		// Hide the current weapon
		Weapons[EquippedWeaponIndex]->SetVisibility(false);

		EquippedWeaponIndex = NewWeaponIndex;
		OnWeaponChangedEvent.Broadcast(NewWeapon);
		NewWeapon->SetVisibility(true);
	}
	else
	{
		NewWeapon->SetVisibility(false);
	}
	OnWeaponAddedEvent.Broadcast(NewWeapon);
}

AGun* UWeaponLoadoutComponent::GetEquippedWeapon()
{
	if (EquippedWeaponIndex >= Weapons.Num())
		return nullptr;

	return Weapons[EquippedWeaponIndex].Get();
}

void UWeaponLoadoutComponent::EquipNextWeapon()
{
	// No need to change weapons if there is only one
	if (Weapons.Num() <= 1)
		return;

	// Hide the current weapon
	if (AGun* EquippedWeapon = GetEquippedWeapon())
	{
		// Prevent changing weapons while reloading
		if (EquippedWeapon->GetIsReloading())
			return;

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

bool UWeaponLoadoutComponent::ReloadWeapon()
{
	if (AGun* EquippedWeapon = GetEquippedWeapon())
	{
		if (EquippedWeapon->GetIsReloading())
			return false;

		EquippedWeapon->StopFiring();
		EquippedWeapon->Reload();
	}

	return true;
}

void UWeaponLoadoutComponent::BeginPlay()
{
	Super::BeginPlay();

	if (AGun* EquippedWeapon = GetEquippedWeapon())
	{
		OnWeaponChangedEvent.Broadcast(EquippedWeapon);
	}
}
