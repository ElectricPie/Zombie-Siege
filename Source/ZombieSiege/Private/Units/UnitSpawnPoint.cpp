// Fill out your copyright notice in the Description page of Project Settings.


#include "Units/UnitSpawnPoint.h"

#include "UnitCharacter.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Rooms/Barricade.h"

// Sets default values
AUnitSpawnPoint::AUnitSpawnPoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionCapsule"));
	CapsuleComponent->ShapeColor = FColor(255, 138, 5, 255);
	CapsuleComponent->bDrawOnlyIfSelected = true;
	CapsuleComponent->InitCapsuleSize(40.0f, 92.0f);
	CapsuleComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	CapsuleComponent->bShouldCollideWhenPlacing = true;
	CapsuleComponent->SetShouldUpdatePhysicsVolume(true);
	CapsuleComponent->Mobility = EComponentMobility::Static;
	RootComponent = CapsuleComponent;
	bCollideWhenPlacing = true;

#if WITH_EDITORONLY_DATA
	SpawnDirectionArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn Direction"));
	if (SpawnDirectionArrow)
	{
		SpawnDirectionArrow->ArrowColor = FColor(150, 200, 255);
		SpawnDirectionArrow->SetupAttachment(RootComponent);
		SpawnDirectionArrow->ArrowSize = 1.0f;
		SpawnDirectionArrow->bTreatAsASprite = true;
		SpawnDirectionArrow->bIsScreenSizeScaled = true;
	}
#endif
}

TWeakObjectPtr<AUnitCharacter> AUnitSpawnPoint::SpawnUnit(const TSubclassOf<AUnitCharacter> UnitClass)
{
	if (bForceDeactivate) return nullptr;
	if (UnitClass == nullptr) return nullptr;
	if (!bIsActive || ActiveBarricades.Num() == 0) return nullptr;
	if (GetWorld() == nullptr) return nullptr;

	// Select barricade for unit
	const int32 BarricadeIndex = FMath::RandRange(0, ActiveBarricades.Num() - 1);
	TWeakObjectPtr<ABarricade> TargetBarricade = ActiveBarricades[BarricadeIndex];
	if (!TargetBarricade.IsValid()) return nullptr;

	const AActor* ActorToFit = UnitClass->GetDefaultObject<AActor>();
	const FVector SpawnLocation = GetActorLocation();
	const FRotator SpawnRotation = GetActorRotation();
	// Check if there is space to spawn the unit
	if (GetWorld()->EncroachingBlockingGeometry(ActorToFit, SpawnLocation, SpawnRotation)) return nullptr;

	// Spawn the unit
	FActorSpawnParameters SpawnParameters;
	TWeakObjectPtr<AUnitCharacter> SpawnedUnit = GetWorld()->SpawnActor<AUnitCharacter>(UnitClass, GetActorLocation(), GetActorRotation());
	SpawnedUnit->SetTargetBarricade(TargetBarricade);
	
	return SpawnedUnit;
}

void AUnitSpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	// Set to active if at least one of the connected barricades is active
	bIsActive = false;
	// Keep track of the active barricades
	for (const auto& Barricade : ConnectedBarricades)
	{
		if (!Barricade.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("%s has an invalid barricade in its connected barricades"), *GetActorNameOrLabel());
			continue;
		}
		if (Barricade->GetIsActive())
		{
			if (!ActiveBarricades.Contains(Barricade))
			{
				ActiveBarricades.Add(Barricade);
			}
		}
		else
		{
			BarricadeChangedHandles.Add(Barricade ,Barricade->OnActiveChangedEvent.AddUObject(this, &AUnitSpawnPoint::OnBarricadeActiveChanged));
		}
	}

	// Enable the spawn point if there is at least one active barricades
	if (ActiveBarricades.Num() > 0)
	{
		bIsActive = true;
	}
}

void AUnitSpawnPoint::OnBarricadeActiveChanged(TWeakObjectPtr<ABarricade> BarricadeChanging, bool bNewActiveState)
{
	if (bNewActiveState)
	{
		bIsActive = true;
		if (!ActiveBarricades.Contains(BarricadeChanging))
		{
			ActiveBarricades.Add(BarricadeChanging);
		}
		if (BarricadeChangedHandles.Contains(BarricadeChanging))
		{
			BarricadeChanging->OnActiveChangedEvent.Remove(BarricadeChangedHandles[BarricadeChanging]);
			BarricadeChangedHandles.Remove(BarricadeChanging);
		}
	}
}
