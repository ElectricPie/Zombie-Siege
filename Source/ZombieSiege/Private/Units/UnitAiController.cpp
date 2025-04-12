// Fill out your copyright notice in the Description page of Project Settings.


#include "Units/UnitAiController.h"

#include "BrainComponent.h"
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

	if (AUnitCharacter* Unit = Cast<AUnitCharacter>(InPawn))
	{
		Unit->OnKilledEvent.AddLambda([this](AUnitCharacter* UnitKilled, AController* KillInstigator, AActor* KillCauser)
		{
			StopBehaviorTree();
		});
	}
}
