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
	
public:
	void StopBehaviorTree() const;
	
protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY(EditAnywhere, Category="AI")
	UBehaviorTree* BehaviorTree;

private:
	UFUNCTION()
	void OnPossessedUnitDeath(AActor* VictimActor, AController* KillerController, AActor* KillerActor);
};
