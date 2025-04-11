// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponBuyPoint.h"

#include "Gun.h"
#include "Interactions/InteractableComponent.h"
#include "Components/MoneyStoreComponent.h"
#include "Components/WeaponLoadoutComponent.h"
#include "Money/MoneyStoreInterface.h"
#include "Weapons/WeaponBuyPointDataAsset.h"

// Sets default values
AWeaponBuyPoint::AWeaponBuyPoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("Interactable Component"));
	RootComponent = InteractableComponent;
	InteractableComponent->SetCanInteract(true);
	InteractableComponent->SetInteractMessage(FText::FromString("Buy weapon"));
	InteractableComponent->OnInteractEvent.AddLambda(
		[this](const AController* InteractionInstigator, const AActor* InteractionCauser)
		{
			if (UMoneyStoreComponent* MoneyStoreComponent = IMoneyStoreInterface::Execute_GetMoneyStoreComponent(InteractionInstigator))
			{
				BuyWeapon(MoneyStoreComponent, InteractionCauser);
			}
		});

	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh Component"));
	WeaponMeshComponent->SetupAttachment(RootComponent);
	WeaponMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMeshComponent->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
}

void AWeaponBuyPoint::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	RefreshWeaponMesh();
}

void AWeaponBuyPoint::RefreshWeaponMesh() const
{
	if (WeaponBuyPointDataAsset && WeaponBuyPointDataAsset->GetWeaponClass())
	{
		const AGun* DefaultWeapon = WeaponBuyPointDataAsset->GetWeaponClass()->GetDefaultObject<AGun>();
		WeaponMeshComponent->SetSkeletalMesh(DefaultWeapon->GetMesh()->GetSkeletalMeshAsset());
	}
	else
	{
		WeaponMeshComponent->SetSkeletalMesh(nullptr);
	}
}

void AWeaponBuyPoint::BuyWeapon(UMoneyStoreComponent* MoneyStore,
                                const AActor* ActorToGiveWeapon)
{
	if (MoneyStore == nullptr || ActorToGiveWeapon == nullptr)
		return;

	// Not enough money
	if (!MoneyStore->TakeMoney(WeaponBuyPointDataAsset->GetCost()))
		return;

	// Adds the weapon to the players loadout
	if (UWeaponLoadoutComponent* WeaponLoadoutComponent = ActorToGiveWeapon->FindComponentByClass<UWeaponLoadoutComponent>())
	{
		AGun* NewWeapon = GetWorld()->SpawnActor<AGun>(WeaponBuyPointDataAsset->GetWeaponClass(), GetActorTransform());
		WeaponLoadoutComponent->AddWeapon(NewWeapon, true);
	}

	// TODO: This is temporary until a it is decided how to handle the weapon ammo and multiplayer
	Destroy();
}
