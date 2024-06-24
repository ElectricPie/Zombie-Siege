// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UnitCharacter.generated.h"

class UBehaviorTree;
UCLASS()
class AUnitCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUnitCharacter();

	// TODO: Attack method called from task
	UFUNCTION(BlueprintCallable)
	void Attack(AActor* Target);

private:
	UPROPERTY(EditAnywhere, Category="Attack", meta=(ClampMin=0.f, UIMin=0.f))
	float AttackDamage = 25.f;
};
