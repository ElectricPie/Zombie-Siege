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

	UFUNCTION(BlueprintCallable, BlueprintPure, Category=Attack)
	float GetAttackRange() const { return AttackRange; };

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Attack, meta=(ClampMin=0.f, UIMin=0.f))
	float AttackDamage = 25.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Attack, meta=(ClampMin=0.f, UIMin=0.f))
	float AttackRange = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Attack, meta=(ClampMin=0.f, UIMin=0.f))
	float AttackDelay = 1.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Attack)
	float LastAttackTime = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Animation)
	UAnimMontage* AttackMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Health)
	float MaxHealth = 40.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Health)
	float CurrentHealth = 40.f;;
};
