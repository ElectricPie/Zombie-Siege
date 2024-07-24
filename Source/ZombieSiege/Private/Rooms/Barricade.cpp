// Fill out your copyright notice in the Description page of Project Settings.


#include "Barricade.h"

#include "NavLinkComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/InteractableComponent.h"
#include "Components/MoneyRewardComponent.h"

#define DEFAULT_BARRICADE_REWARD 40
#define DEFAULT_BARRICADE_TIME_BETWEEN_REWARDS 5

// Sets default values
ABarricade::ABarricade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BaseComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = BaseComponent;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCanEverAffectNavigation(false);
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
	
	PlayerInteractionTrigger = CreateDefaultSubobject<UInteractableComponent>(TEXT("Inside Interactable"));
	PlayerInteractionTrigger->SetupAttachment(RootComponent);
	PlayerInteractionTrigger->OnInteractEvent.AddUObject(this, &ABarricade::OnInteract);
	
	InsideDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("Inside Direction Arrow"));
	InsideDirection->SetupAttachment(RootComponent);

	MoneyRewardComponent = CreateDefaultSubobject<UMoneyRewardComponent>(TEXT("Money Reward"));
	MoneyRewardComponent->SetAmountToGive(DEFAULT_BARRICADE_REWARD);
	MoneyRewardComponent->SetTimeBetweenRewards(DEFAULT_BARRICADE_TIME_BETWEEN_REWARDS);

	NavLinkComponent = CreateDefaultSubobject<UNavLinkComponent>(TEXT("Nav Link Component"));
	NavLinkComponent->SetupAttachment(RootComponent);
	NavLinkComponent->Links.Empty();
	// Left is outside, right is inside
	FNavigationLink Link;
	Link.Left = FVector(-120.f, 0.f, 0.f);
	Link.Right = FVector(120.f, 0.f, 0.f);
	Link.Direction = ENavLinkDirection::LeftToRight;
	NavLinkComponent->Links.Add(Link);
}

float ABarricade::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if (IsDestroyed()) return 0.f;

	if (UStaticMeshComponent* Plank = Planks[DestroyedPlanks])
	{
		Plank->SetVisibility(false);
		DestroyedPlanks++;
	}

	PlayerInteractionTrigger->SetCanInteract(true);
	
	return DamageAmount;
}

void ABarricade::Repair()
{
	if (AgentsCrossing.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Unit is crossing barricade, cannot repair!"));
		return;
	}
	
	for (const auto & Plank : Planks)
	{
		Plank->SetVisibility(true);
	}
	DestroyedPlanks = 0;

	PlayerInteractionTrigger->SetCanInteract(false);
}

void ABarricade::SetIsActive(const bool bNewIsActive)
{
	bIsActive = bNewIsActive;
	OnActiveChangedEvent.Broadcast(this, bIsActive);
}

FVector ABarricade::GetInsideLocation() const
{
	FVector InsidePos = FVector::ZeroVector;
	if (NavLinkComponent->Links.Num() > 0)
	{
		InsidePos = NavLinkComponent->GetComponentTransform().TransformPosition(NavLinkComponent->Links[0].Right);
	}
	return InsidePos;
}

FVector ABarricade::GetOutsideLocation() const
{
	FVector OutSidePos = FVector::ZeroVector;
	if (NavLinkComponent->Links.Num() > 0)
	{
		OutSidePos = NavLinkComponent->GetComponentTransform().TransformPosition(NavLinkComponent->Links[0].Left);
	}
	return OutSidePos;
}

void ABarricade::StartCrossing(AActor* Agent)
{
	if (AgentsCrossing.Contains(Agent)) return;
	AgentsCrossing.Add(Agent);
	Agent->OnDestroyed.AddUniqueDynamic(this, &ABarricade::StopCrossing);
}

void ABarricade::StopCrossing(AActor* Agent)
{
	if (AgentsCrossing.Contains(Agent))
	{
		AgentsCrossing.Remove(Agent);
		if (Agent->IsValidLowLevel())
		{
			Agent->OnDestroyed.RemoveDynamic(this, &ABarricade::StopCrossing);
		}
	}
}

// Called when the game starts or when spawned
void ABarricade::BeginPlay()
{
	Super::BeginPlay();
}

void ABarricade::OnInteract(TWeakObjectPtr<AController> InteractionInstigator, TWeakObjectPtr<AActor> InteractionCauser)
{
	Repair();
	MoneyRewardComponent->RewardMoney(InteractionInstigator.Get());
}
