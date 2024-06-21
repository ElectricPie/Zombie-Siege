// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/UnitAiController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"
#include "Units/UnitCharacter.h"

void AUnitAiController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (Target != nullptr) return;
	
	if (AActor* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		Target = PlayerCharacter;
		MoveToActor(Target.Get(), -1.f, true, true, false, 0, true);
	
	}
}