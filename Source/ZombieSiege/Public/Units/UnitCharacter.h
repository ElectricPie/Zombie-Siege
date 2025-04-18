// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Health/HealthComponentInterface.h"
#include "UnitCharacter.generated.h"

class UHealthComponent;
class ABarricade;
class UBehaviorTree;
class UFMODEvent;
class UMoneyRewardComponent;

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnUnitKilledSingature, AUnitCharacter* /*UnitKilled*/, AController* /*KillInstigator*/, AActor* /*KillCauser*/)

UCLASS()
class AUnitCharacter : public ACharacter, public IHealthComponentInterface
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
	void SetTargetBarricade(ABarricade* NewTargetBarricade);

	UFUNCTION(BlueprintPure, Category="Money")
	UMoneyRewardComponent* GetMoneyRewardComponent() const { return MoneyRewardComponent; }

	/* HealthComponentInterface */
	virtual UHealthComponent* GetHealthComponent_Implementation() const override;
	/* End HealthComponentInterface */
	
public:
	FOnUnitKilledSingature OnKilledEvent;

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
	TObjectPtr<UAnimMontage> AttackMontage;

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(VisibleAnywhere, Category="Money")
	TObjectPtr<UMoneyRewardComponent> MoneyRewardComponent;
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UHealthComponent> HealthComponent;
	UPROPERTY(VisibleAnywhere, Category="Target")
	TObjectPtr<ABarricade> TargetBarricade;
	
	UPROPERTY(EditAnywhere, Category="Sound")
	TObjectPtr<UFMODEvent> DeathSound;
	UPROPERTY(EditAnywhere, Category="Sound")
	TObjectPtr<UFMODEvent> AttackSound;
	UPROPERTY(EditAnywhere, Category="Sound")
	TObjectPtr<UFMODEvent> TakeDamageSound;

	UPROPERTY(EditAnywhere)
	float DeathLifeSpan = 5.f;
	
private:
	UFUNCTION()
	void Die_Server(AController* KillInstigator, AActor* KillCauser);
	void HealthChange_Client(const float NewCurrentHealth);

	void Ragdoll();
};
