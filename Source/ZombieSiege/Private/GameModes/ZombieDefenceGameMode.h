// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ZombieDefenceGameMode.generated.h"

class AUnitSpawnPoint;
class AUnitCharacter;


/**
 * 
 */
UCLASS()
class AZombieDefenceGameMode : public AGameMode
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void OnPostLogin(AController* NewPlayer) override;

private:
	void OnUnitKilled(TWeakObjectPtr<AUnitCharacter> UnitKilled, TWeakObjectPtr<AController> KillerInstigator, TWeakObjectPtr<AActor> KillCauser);

	int32 RoundUnitCountBelow20();
	int32 RoundUnitCount20AndAbove();
	
	void GetActiveUnitSpawnPoints();
	void SpawnUnit();

	void StartNewRound();
	void ResetRoundStats();
	
private:
	UPROPERTY(EditAnywhere, Category="Player", meta=(ClampMin=0, UIMin=0))
	int32 StartingMoney = 500;

	UPROPERTY(VisibleAnywhere, Category="Round")
	int32 RoundNumber = 1;
	
	UPROPERTY(EditAnywhere, Category="Spawning")
	TSubclassOf<AUnitCharacter> UnitClass;
	UPROPERTY(EditAnywhere, Category="Spawning", meta=(ClampMin=1, UIMin=1, ToolTip="The maximum amount of units that can be spawned in at one time"))
	int32 MaxCurrentSpawnedUnits = 20;
	UPROPERTY(EditAnywhere, Category="Spawning", meta=(ClampMin=1, UIMin=1, ToolTip="The number of units in the first round"))
	int32 InitialUnitCount = 6;
	UPROPERTY(VisibleAnywhere, Category="Spawning")
	int32 UnitsToBeSpawnedThisRound = 0;
	UPROPERTY(VisibleAnywhere, Category="Spawning")
	int32 UnitsSpawnedThisRound = 0;
	UPROPERTY(VisibleAnywhere, Category="Spawning")
	int32 UnitsKilledThisRound = 0;
	UPROPERTY(EditAnywhere, Category="Spawning", meta=(ClampMin=1.f, UIMin=1.f, ToolTip="The amount of time after the game starts before units start spawning"))
	float RoundStartDelay = 4.f;
	UPROPERTY(EditAnywhere, Category="Spawning", meta=(ClampMin=0.f, UIMin=0.f, ToolTip="The initial time between units spawning"))
	float InitialSpawnDelay = 2.f;
	float CurrentSpawnDelay = 2.f;
	UPROPERTY(EditAnywhere, Category="Spawning", meta=(ClampMin=0.f, UIMin=0.f, ToolTip="The minimum amount of time between units spawning"))
	float MinSpawnDelay = 0.5f;
	
	UPROPERTY(VisibleAnywhere)
	TArray<TWeakObjectPtr<AUnitSpawnPoint>> ActiveSpawnPoints;
	TSet<TWeakObjectPtr<AUnitCharacter>> ActiveUnits;

	FTimerHandle RoundSpawnTimerHandle;
};
