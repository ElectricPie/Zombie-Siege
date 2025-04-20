// Fill out your copyright notice in the Description page of Project Settings.


#include "Units/UnitAiController.h"

#include "BrainComponent.h"
#include "Health/HealthComponent.h"
#include "Units/UnitCharacter.h"

class AUnitCharacter;

void AUnitAiController::StopBehaviorTree() const
{
	if (BrainComponent)
	{
		BrainComponent->StopLogic("GameOver");
	}
}

void AUnitAiController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BehaviorTree);
}

void AUnitAiController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (const AUnitCharacter* Unit = Cast<AUnitCharacter>(InPawn))
	{
		Unit->GetHealthComponent_Implementation()->OnDeathEvent.AddDynamic(this, &AUnitAiController::OnPossessedUnitDeath);
	}
}

void AUnitAiController::OnPossessedUnitDeath(AActor* VictimActor, AController* KillerController, AActor* KillerActor)
{
	StopBehaviorTree();
}
