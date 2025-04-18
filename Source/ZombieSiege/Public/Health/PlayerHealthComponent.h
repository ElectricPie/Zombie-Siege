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
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UFMODAudioComponent> HealthSoundComponent;
	
public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere, Category="Sound")
	TObjectPtr<UFMODEvent> HealthSound;
	UPROPERTY(EditAnywhere, Category="Sound")
	FName HealthSoundParameterName = TEXT("Health");
	
};
