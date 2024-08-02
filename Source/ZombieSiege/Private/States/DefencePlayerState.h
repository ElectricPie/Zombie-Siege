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

	UFUNCTION(BlueprintGetter, Category="Player")
	int32 GetTotalKills() const { return TotalKills; }
	UFUNCTION(BlueprintGetter, Category="Player")
	int32 GetTotalDeaths() const { return TotalDeaths; }

private:
	UPROPERTY(VisibleAnywhere, Category="Player", BlueprintGetter=GetTotalKills)
	int32 TotalKills = 0;
	UPROPERTY(VisibleAnywhere, Category="Player", BlueprintGetter=GetTotalDeaths)
	int32 TotalDeaths = 0;
};
