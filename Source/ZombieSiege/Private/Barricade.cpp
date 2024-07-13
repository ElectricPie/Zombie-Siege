// Fill out your copyright notice in the Description page of Project Settings.


#include "Barricade.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/InteractableComponent.h"
#include "Components/MoneyRewardComponent.h"
#include "Player/PlayerCharacter.h"

#define DEFAULT_BARRICADE_REWARD 40

// Sets default values
ABarricade::ABarricade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = BaseComponent;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCanEverAffectNavigation(false);
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
	
	PlayerInteractionTrigger = CreateDefaultSubobject<UInteractableComponent>(TEXT("Inside Interactable"));
	PlayerInteractionTrigger->SetupAttachment(RootComponent);
	
	InsideDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("Inside Direction Arrow"));
	InsideDirection->SetupAttachment(RootComponent);

	MoneyRewardComponent = CreateDefaultSubobject<UMoneyRewardComponent>(TEXT("Money Reward"));
	MoneyRewardComponent->SetAmountToGive(DEFAULT_BARRICADE_REWARD);
}

float ABarricade::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	float const DamageDealt = FMath::Clamp(DamageAmount, 0.f, CurrentHealth); 
	
	if (CurrentHealth > 0)
	{
		CurrentHealth -= DamageDealt;
	}

	if (CurrentHealth <= 0)
	{
		Mesh->SetVisibility(false);
	}
	
	PlayerInteractionTrigger->SetDisplayMessage(true);

	return DamageDealt;
}

void ABarricade::Repair()
{
	CurrentHealth = MaxHealth;
	Mesh->SetVisibility(true);

	PlayerInteractionTrigger->SetDisplayMessage(false);
}

// Called when the game starts or when spawned
void ABarricade::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	PlayerInteractionTrigger->OnInteractEvent.AddUObject(this, &ABarricade::OnInteract);
}

void ABarricade::OnInteract(APlayerCharacter* InteractingPlayer)
{
	Repair();
	MoneyRewardComponent->RewardMoney(InteractingPlayer->GetController());
}
