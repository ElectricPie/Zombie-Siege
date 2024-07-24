// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponBuyPoint.h"

#include "Components/InteractableComponent.h"

// Sets default values
AWeaponBuyPoint::AWeaponBuyPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("Interactable Component"));
	RootComponent = InteractableComponent;
	InteractableComponent->SetCanInteract(true);
	InteractableComponent->SetInteractMessage(FText::FromString("buy weapon"));
}

