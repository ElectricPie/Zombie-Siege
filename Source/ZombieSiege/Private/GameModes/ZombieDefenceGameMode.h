// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ZombieDefenceGameMode.generated.h"

class AUnitSpawnPoint;
class AUnitCharacter;
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnMoneyChangedSinganture, int32 /*NewMoneyAmount*/, int32 /*AmountChanged*/);

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
	UPROPERTY(EditAnywhere, Category="Player", meta=(ClampMin=0, UIMin=0))
	int32 StartingMoney = 500;

	// UPROPERTY(EditAnywhere, Category="Units")
	// TSubclassOf<AUnitCharacter> UnitClass;
	UPROPERTY(EditAnywhere, Category="Units", meta=(ClampMin=1, UIMin=1, ToolTip="The maximum amount of units that can be spawned in at one time"))
	int32 MaxCurrentSpawnedUnits = 20;
	UPROPERTY(EditAnywhere, Category="Units", meta=(ClampMin=1, UIMin=1, ToolTip="The number of units in the first wave"))
	int32 InitialUnitCount = 6;
	UPROPERTY(EditAnywhere, Category="Round")
	int32 RoundNumber = 1;

	UPROPERTY(VisibleAnywhere)
	TArray<TWeakObjectPtr<AUnitSpawnPoint>> ActiveSpawnPoints;
	TArray<TWeakObjectPtr<AUnitCharacter>> ActiveUnits;

	int32 WaveCountBelow20();
	int32 WaveCount20AndAbove();
	
	void GetActiveUnitSpawnPoints();
};
