// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponBuyPoint.h"

#include "Weapons/GunBase.h"
#include "Interactions/InteractableComponent.h"
#include "Components/MoneyStoreComponent.h"
#include "Components/WeaponLoadoutComponent.h"
#include "Money/MoneyStoreInterface.h"
#include "Weapons/WeaponBuyPointDataAsset.h"
#include "Weapons/WeaponStatsDataAsset.h"

#define DEFAULT_WEAPON_INTERACT_MESSAGE "Buy weapon"

// Sets default values
AWeaponBuyPoint::AWeaponBuyPoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("Interactable Component"));
	RootComponent = InteractableComponent;
	InteractableComponent->SetCanInteract(true);
	InteractableComponent->SetInteractMessage(FText::FromString(DEFAULT_WEAPON_INTERACT_MESSAGE));

	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh Component"));
	WeaponMeshComponent->SetupAttachment(RootComponent);
	WeaponMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMeshComponent->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));

	bReplicates = false;
}

void AWeaponBuyPoint::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	RefreshWeaponMesh();
}

void AWeaponBuyPoint::BeginPlay()
{
	Super::BeginPlay();

	const FText DisplayName = WeaponBuyPointDataAsset->GetWeaponClass()->GetDefaultObject<AGunBase>()->GetWeaponStats()->GetDisplayName();
	const FText InteractMessage = FText::FromString(FString::Printf(TEXT("Buy %s [Costs %d]"), *DisplayName.ToString(), WeaponBuyPointDataAsset->GetCost()));
	InteractableComponent->SetInteractMessage(InteractMessage);
	
	InteractableComponent->OnInteractableConsumedEvent.AddLambda([this]()
		{
			SetActorHiddenInGame(true);
		});
	
	// Server only
	if (HasAuthority())
	{
		InteractableComponent->OnInteractEvent.AddLambda(
		   [this](const AController* InteractionInstigator, APawn* InteractionCauser)
		   {
			   if (UMoneyStoreComponent* MoneyStoreComponent = IMoneyStoreInterface::Execute_GetMoneyStoreComponent(InteractionInstigator))
			   {
				   TryBuyWeapon_Server(MoneyStoreComponent, InteractionCauser);
			   }
		   });
		return;
	}
}

void AWeaponBuyPoint::RefreshWeaponMesh() const
{
	if (WeaponBuyPointDataAsset && WeaponBuyPointDataAsset->GetWeaponClass())
	{
		const AGunBase* DefaultWeapon = WeaponBuyPointDataAsset->GetWeaponClass()->GetDefaultObject<AGunBase>();
		WeaponMeshComponent->SetSkeletalMesh(DefaultWeapon->GetMesh()->GetSkeletalMeshAsset());
	}
	else
	{
		WeaponMeshComponent->SetSkeletalMesh(nullptr);
	}
}

bool AWeaponBuyPoint::TryBuyWeapon_Server(UMoneyStoreComponent* MoneyStore, APawn* ActorToGiveWeapon)
{
	check(HasAuthority());

	if (MoneyStore == nullptr || ActorToGiveWeapon == nullptr)
		return false;

	// Not enough money
	if (!MoneyStore->TakeMoney_Server(WeaponBuyPointDataAsset->GetCost()))
		return false;

	// Adds the weapon to the players loadout
	if (UWeaponLoadoutComponent* WeaponLoadoutComponent = ActorToGiveWeapon->FindComponentByClass<
		UWeaponLoadoutComponent>())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = ActorToGiveWeapon;
		SpawnParams.Instigator = ActorToGiveWeapon;
		AGunBase* NewWeapon = GetWorld()->SpawnActor<AGunBase>(WeaponBuyPointDataAsset->GetWeaponClass(), GetActorTransform(), SpawnParams);
		WeaponLoadoutComponent->AddWeapon_Server(NewWeapon, true);

		InteractableComponent->InteractionSuccessful();

		return true;
	}

	return false;
}
