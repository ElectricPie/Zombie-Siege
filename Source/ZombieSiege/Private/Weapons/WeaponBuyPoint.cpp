// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponBuyPoint.h"

#include "Components/InteractableComponent.h"
#include "Components/MoneyStoreComponent.h"

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
}

void AWeaponBuyPoint::BuyWeapon(TWeakObjectPtr<AController> InteractionInstigator,
                                TWeakObjectPtr<AActor> InteractionCauser)
{
	if (!InteractionInstigator.IsValid()) return;

	if (UMoneyStoreComponent* MoneyStoreComponent = InteractionInstigator->GetComponentByClass<UMoneyStoreComponent>())
	{
		// Not enough money
		if (!MoneyStoreComponent->TakeMoney(Cost)) return;

		// TODO: Implement giving the weapon to the player

		// TODO: This is temporary until a it is decided how to handle the weapon ammo and multiplayer
		Destroy();
	}
}
