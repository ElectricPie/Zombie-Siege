// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Windows/AllowWindowsPlatformTypes.h"
#include "HealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthValueChangedSignature, const float /*NewHealthPercentage*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnDeathSignature, AController* /*KillInstigator*/, AActor* /*KillCauser*/);

class UFMODEvent;
class UFMODAudioComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FOnHealthValueChangedSignature OnHealthPercentageChangedEvent;
	FOnHealthValueChangedSignature OnCurrentHealthChangedEvent;
	FOnDeathSignature OnDeathEvent;
	
public:	
	// Sets default values for this component's properties
	UHealthComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category="Health")
	void SetEnableHealthRegen(const bool bEnable) { bEnableHealthRegen = bEnable; }

	UFUNCTION(BlueprintCallable, Category="Health")
	float GetMaxHealth() const { return MaxHealth; }
	UFUNCTION(BlueprintPure, Category="Health")
	float GetCurrentHealth() const { return CurrentHealth; }
	UFUNCTION(BlueprintPure, Category="Health")
	float GetHealthPercentage() const { return CurrentHealth / MaxHealth; }
	/**
	 * @brief Sets the max health
	 * @param NewMaxHealth The new maximum health value
	 * @param bKeepHealthPercentage If true, the health percentage will be kept the same, otherwise the current health will remain the same
	 */
	UFUNCTION(BlueprintCallable, Category="Health")
	void SetMaxHealth(const float NewMaxHealth, const bool bKeepHealthPercentage = true);
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(Replicated, EditAnywhere, Category="Health")
	float MaxHealth = 30.f;
	UPROPERTY(ReplicatedUsing=OnRep_CurrentHealth, VisibleAnywhere, Category="Health")
	float CurrentHealth = 30.f;
	UPROPERTY(EditAnywhere, Category="Health", meta=(ClampMin=0, UIMin=0, ToolTip="The rate at which health regenerates per second"))
	float HealthRegenRate = 0.f;
	UPROPERTY(EditAnywhere, Category="Health")
	bool bEnableHealthRegen = false;
	
	UPROPERTY(EditAnywhere, Category="Sound")
	TObjectPtr<UFMODEvent> HitSound;
	UPROPERTY(EditAnywhere, Category="Sound")
	TObjectPtr<UFMODEvent> DeathSound;
	UPROPERTY(EditAnywhere, Category="Sound")
	TObjectPtr<UFMODEvent> HealthSound;
	UPROPERTY(EditAnywhere, Category="Sound")
	FName HealthSoundParameterName = TEXT("Health");
	UPROPERTY()
	TObjectPtr<UFMODAudioComponent> HealthSoundComponent = nullptr;
	
	TWeakObjectPtr<UFMODAudioComponent> HitSoundComponent = nullptr;

private:
	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void OnRep_CurrentHealth();
};
