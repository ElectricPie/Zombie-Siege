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
	void AddDeath();
	void AddKill();

private:
	UPROPERTY(VisibleAnywhere)
	int32 TotalScore = 0;
	UPROPERTY(VisibleAnywhere)
	int32 TotalKills = 0;
	UPROPERTY(VisibleAnywhere)
	int32 TotalDeaths = 0;
};
