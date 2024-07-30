// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/UnitAiController.h"

#include "BrainComponent.h"


void AUnitAiController::StopBehaviorTree()
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
