// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "UnitAiController.generated.h"

/**
 * 
 */
UCLASS()
class AUnitAiController : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void Tick(float DeltaSeconds) override;

private:
	TWeakObjectPtr<AActor> Target;
};
