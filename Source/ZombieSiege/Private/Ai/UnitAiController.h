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
	void StopBehaviorTree();
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category="AI")
	UBehaviorTree* BehaviorTree;
};
