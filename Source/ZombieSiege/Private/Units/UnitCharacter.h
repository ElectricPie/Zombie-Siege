// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UnitCharacter.generated.h"

class UHealthComponent;
class ABarricade;
class UBehaviorTree;
class UFMODEvent;
class UMoneyRewardComponent;

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnUnitKilledSingature, AUnitCharacter* /*UnitKilled*/, AController* /*KillInstigator*/, AActor* /*KillCauser*/)

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

	UFUNCTION(BlueprintPure, Category="Target")
	ABarricade* GetTargetBarricade() const { return TargetBarricade.Get(); }
	void SetTargetBarricade(TWeakObjectPtr<ABarricade> NewTargetBarricade);

	UMoneyRewardComponent* GetMoneyRewardComponent() const { return MoneyRewardComponent; }
	
public:
	FOnUnitKilledSingature OnKilledEvent;
	
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Attack", meta=(ClampMin=0.f, UIMin=0.f))
	float AttackDamage = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Attack", meta=(ClampMin=0.f, UIMin=0.f))
	float AttackRange = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Attack", meta=(ClampMin=0.f, UIMin=0.f))
	float AttackDelay = 1.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Attack")
	float LastAttackTime = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Animation")
	UAnimMontage* AttackMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
	float MaxHealth = 40.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Health")
	float CurrentHealth = 40.f;

private:
	UFUNCTION()
	void Die(AController* KillInstigator, AActor* KillCauser);

private:
	UPROPERTY(VisibleAnywhere, Category="Money")
	TObjectPtr<UMoneyRewardComponent> MoneyRewardComponent;
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UHealthComponent> HealthComponent;
	UPROPERTY(VisibleAnywhere, Category="Target")
	TWeakObjectPtr<ABarricade> TargetBarricade;
	
	UPROPERTY(EditAnywhere, Category="Sound")
	UFMODEvent* DeathSound;
	UPROPERTY(EditAnywhere, Category="Sound")
	UFMODEvent* AttackSound;
	UPROPERTY(EditAnywhere, Category="Sound")
	UFMODEvent* TakeDamageSound;

};
