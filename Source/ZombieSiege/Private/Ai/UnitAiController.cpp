// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/UnitAiController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"
#include "Units/UnitCharacter.h"

void AUnitAiController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BehaviorTree);
}
