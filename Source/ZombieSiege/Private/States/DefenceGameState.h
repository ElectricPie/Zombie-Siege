// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "DefenceGameState.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnRoundChangedSignature, int32 /*RoundNumber*/);

/**
 * 
 */
UCLASS()
class ADefenceGameState : public AGameState
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="Round")
	int32 GetCurrentRound() const { return CurrentRound; }
	void StartNextRound();

public:
	FOnRoundChangedSignature OnRoundChangedEvent;
	
private:
	UPROPERTY(VisibleAnywhere, Category="Round")
	int32 CurrentRound = 0;
};
