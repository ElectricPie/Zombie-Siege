// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponBuyPoint.h"

#include "Gun.h"
#include "Components/InteractableComponent.h"
#include "Components/MoneyStoreComponent.h"
#include "Components/WeaponLoadoutComponent.h"

// Sets default values
AWeaponBuyPoint::AWeaponBuyPoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("Interactable Component"));
	RootComponent = InteractableComponent;
	InteractableComponent->SetCanInteract(true);
	InteractableComponent->SetInteractMessage(FText::FromString("buy weapon"));
	InteractableComponent->OnInteractEvent.AddUObject(this, &AWeaponBuyPoint::BuyWeapon);
	
	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh Component"));
	WeaponMeshComponent->SetupAttachment(RootComponent);
	WeaponMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMeshComponent->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
}

void AWeaponBuyPoint::BuyWeapon(TWeakObjectPtr<AController> InteractionInstigator,
                                TWeakObjectPtr<AActor> InteractionCauser)
{
	if (!InteractionInstigator.IsValid())
		return;

	// TODO: Update to work with new interface
	if (UMoneyStoreComponent* MoneyStoreComponent = InteractionInstigator->GetComponentByClass<UMoneyStoreComponent>())
	{
		// Not enough money
		if (!MoneyStoreComponent->TakeMoney(Cost)) return;

		// Adds the weapon to the players loadout
		if (UWeaponLoadoutComponent* WeaponLoadoutComponent = InteractionCauser->FindComponentByClass<UWeaponLoadoutComponent>())
		{
			AGun* NewWeapon = GetWorld()->SpawnActor<AGun>(WeaponClass, GetActorTransform());
			WeaponLoadoutComponent->AddWeapon(NewWeapon, true);
		}

		// TODO: This is temporary until a it is decided how to handle the weapon ammo and multiplayer
		Destroy();
	}
}
