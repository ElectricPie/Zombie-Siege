// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDeathSignature, AController*, KillInstigator, AActor*, KillCauser);

class UFMODEvent;
class UFMODAudioComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	virtual void BeginPlay() override;

public:
	FOnDeathSignature OnDeathEvent;

private:
	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

private:
	UPROPERTY(EditAnywhere, Category="Health")
	float MaxHealth = 30.f;
	UPROPERTY(VisibleAnywhere, Category="Health")
	float CurrentHealth = 30.f;
	
	UPROPERTY(EditAnywhere, Category="Sound")
	UFMODEvent* HitSound;
	TWeakObjectPtr<UFMODAudioComponent> HitSoundComponent = nullptr;
	UPROPERTY(EditAnywhere, Category="Sound")
	UFMODEvent* DeathSound;
};
