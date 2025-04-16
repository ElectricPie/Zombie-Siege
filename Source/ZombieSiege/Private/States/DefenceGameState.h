// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "DefenceGameState.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameModeRoundChangedSignature, const int32 /*RoundNumber*/);

/**
 * 
 */
UCLASS()
class ADefenceGameState : public AGameState
{
	GENERATED_BODY()

public:
	FOnGameModeRoundChangedSignature OnRoundChangedEvent;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(BlueprintPure, Category="Round")
	int32 GetCurrentRound() const { return CurrentRound; }
	void StartNextRound();
	
private:
	UPROPERTY(ReplicatedUsing=OnRep_CurrentRound, VisibleAnywhere, Category="Round")
	int32 CurrentRound = 0;

private:
	UFUNCTION()
	void OnRep_CurrentRound() const;
};
