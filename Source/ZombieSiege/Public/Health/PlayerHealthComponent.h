// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Health/HealthComponent.h"
#include "PlayerHealthComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ZOMBIESIEGE_API UPlayerHealthComponent : public UHealthComponent
{
	GENERATED_BODY()

public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;
	virtual void OnRep_CurrentHealth() override;
	
private:
	UPROPERTY()
	TObjectPtr<UFMODEvent> HealthSound;
	UPROPERTY(EditAnywhere, Category="Sound")
	FName HealthSoundParameterName = TEXT("Health");
	UPROPERTY()
	TObjectPtr<UFMODAudioComponent> HealthSoundComponent;

private:
	void UpdateHealthSoundParameter() const;
	
};
