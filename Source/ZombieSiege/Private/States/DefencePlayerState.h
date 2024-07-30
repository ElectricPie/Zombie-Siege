// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "DefencePlayerState.generated.h"

class UMoneyStoreComponent;

/**
 * 
 */
UCLASS()
class ADefencePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	void AddDeath() { TotalDeaths++; }
	void AddKill() { TotalKills++; }

	UFUNCTION(BlueprintPure, Category="Player")
	int32 GetTotalKills() const { return TotalKills; }
	UFUNCTION(BlueprintPure, Category="Player")
	int32 GetTotalDeaths() const { return TotalDeaths; }

private:
	UPROPERTY(VisibleAnywhere, Category="Player")
	int32 TotalKills = 0;
	UPROPERTY(VisibleAnywhere, Category="Player")
	int32 TotalDeaths = 0;
};
