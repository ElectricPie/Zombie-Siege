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

	UFUNCTION(BlueprintCallable)
	void Attack(AActor* Target);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Attack")
	float GetAttackRange() const { return AttackRange; };

private:
	UPROPERTY(EditAnywhere, Category="Attack", meta=(ClampMin=0.f, UIMin=0.f))
	float AttackDamage = 25.f;
	UPROPERTY(EditAnywhere, Category="Attack", meta=(ClampMin=0.f, UIMin=0.f))
	float AttackRange = 100.f;
};
