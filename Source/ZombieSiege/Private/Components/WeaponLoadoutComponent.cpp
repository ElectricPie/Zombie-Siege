// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WeaponLoadoutComponent.h"

#include "Net/UnrealNetwork.h"
#include "Weapons/GunBase.h"

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

void UWeaponLoadoutComponent::Fire()
{
	if (AGunBase* Weapon = GetEquippedWeapon())
	{
		Weapon->Fire();
	}
}

void UWeaponLoadoutComponent::StopFiring()
{
	if (AGunBase* Weapon = GetEquippedWeapon())
	{
		Weapon->StopFiring();
	}
}

bool UWeaponLoadoutComponent::Reload()
{
	if (AGunBase* EquippedWeapon = GetEquippedWeapon())
	{
		if (EquippedWeapon->GetIsReloading())
			return false;

		EquippedWeapon->StopFiring();
		EquippedWeapon->Reload();
	}

	return true;
}

void UWeaponLoadoutComponent::AddWeapon_Server(AGunBase* NewWeapon, bool bEquip /*= false*/)
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

	if (AGunBase* EquippedWeapon = GetEquippedWeapon())
	{
		// EquippedWeapon->CancelReload();
	}

	const int32 NewWeaponIndex = Weapons.Add(NewWeapon);
	if (bEquip)
	{
		// Hide the current weapon if one is equipped
		if (Weapons.Num() > 0)
		{
			Weapons[NewWeaponIndex]->SetVisibility(false);
		}

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

AGunBase* UWeaponLoadoutComponent::GetEquippedWeapon()
{
	if (EquippedWeaponIndex >= Weapons.Num() || EquippedWeaponIndex < 0)
		return nullptr;

	return Weapons[EquippedWeaponIndex].Get();
}

void UWeaponLoadoutComponent::ServerEquipNextWeapon_Implementation()
{
	// No need to change weapons if there is only one
	if (Weapons.Num() <= 1)
		return;

	// Hide the current weapon if one is equipped
	if (const AGunBase* EquippedWeapon = GetEquippedWeapon())
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
	if (AGunBase* EquippedWeapon = GetEquippedWeapon())
	{
		OnWeaponChangedEvent.Broadcast(EquippedWeapon);
		EquippedWeapon->SetVisibility(true);
	}
	else
	{
		OnWeaponChangedEvent.Broadcast(nullptr);
	}
}

void UWeaponLoadoutComponent::BeginPlay()
{
	Super::BeginPlay();

	if (AGunBase* EquippedWeapon = GetEquippedWeapon())
	{
		OnWeaponChangedEvent.Broadcast(EquippedWeapon);
	}
}

void UWeaponLoadoutComponent::OnRep_EquippedWeaponIndex(const int32 OldEquippedWeaponIndex)
{
	if (const AGunBase* OldEquippedWeapon = Weapons.IsValidIndex(OldEquippedWeaponIndex) ? Weapons[OldEquippedWeaponIndex] : nullptr)
	{
		OldEquippedWeapon->SetVisibility(false);
	}
	
	OnWeaponChangedEvent.Broadcast(GetEquippedWeapon());

	if (AGunBase* EquippedWeapon = GetEquippedWeapon())
	{
		OnWeaponChangedEvent.Broadcast(EquippedWeapon);
		EquippedWeapon->SetVisibility(true);
	}
	else
	{
		OnWeaponChangedEvent.Broadcast(nullptr);
	}
}

void UWeaponLoadoutComponent::OnRep_Weapons()
{
	// Need to be called if any weapons change order or are added/removed
	OnWeaponChangedEvent.Broadcast(GetEquippedWeapon());
}
