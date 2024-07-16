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

void AUnitSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
	// Set to active if at least one of the connected barricades is active
	bIsActive = false;
	UE_LOG(LogTemp, Warning, TEXT("Connected Barricades %d"), ConnectedBarricades.Num());
	for (const auto& Barricade : ConnectedBarricades)
	{
		if (Barricade->GetIsActive())
		{
			bIsActive = true;
			// Barricade->OnActiveChangedEvent.AddUObject(this, &AUnitSpawnPoint::OnBarricadeActiveChanged);
			return;
		}
	}
}
