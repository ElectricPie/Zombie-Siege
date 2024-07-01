// Fill out your copyright notice in the Description page of Project Settings.


#include "Barricade.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/InteractableComponent.h"
#include "Player/PlayerCharacter.h"

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
	
	OuterTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Outside Trigger"));
	OuterTrigger->SetupAttachment(RootComponent);
	
	InsideTrigger = CreateDefaultSubobject<UInteractableComponent>(TEXT("Inside Interactable"));
	InsideTrigger->SetupAttachment(RootComponent);
	InsideTrigger->OnInteractEvent.AddUObject(this, &ABarricade::OnInteract);
	
	InsideDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("Inside Direction Arrow"));
	InsideDirection->SetupAttachment(RootComponent);
}

FVector ABarricade::GetOutsideTriggerPos() const
{
	return OuterTrigger->GetComponentLocation();
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

	return DamageDealt;
}

void ABarricade::Repair()
{
	CurrentHealth = MaxHealth;
}

// Called when the game starts or when spawned
void ABarricade::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
}

void ABarricade::OnInteract(APlayerCharacter* InteractingPlayer)
{
	Repair();
}
