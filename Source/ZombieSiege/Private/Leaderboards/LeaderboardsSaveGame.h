// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LeaderboardsSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FLeaderboardData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString PlayerName;
	UPROPERTY(BlueprintReadWrite)
	int32 RoundsSurvived;
	UPROPERTY(BlueprintReadWrite)
	int32 Kills;
	UPROPERTY(BlueprintReadWrite)
	int32 Score;
	UPROPERTY(BlueprintReadWrite)
	int32 Deaths;
	UPROPERTY(BlueprintReadWrite)
	FDateTime DateAchieved;
};

/**
 * 
 */
UCLASS()
class ULeaderboardsSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<FLeaderboardData> LeaderboardData;
};
