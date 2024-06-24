// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/UnitAiController.h"

void AUnitAiController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BehaviorTree);
}
