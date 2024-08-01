// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameSaveSubsystem.generated.h"

struct FLeaderboardData;
class ULeaderboardsSaveGame;
/**
 * 
 */
UCLASS()
class UGameSaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Game Save")
	void AddLeaderboardEntry(const FLeaderboardData& Data);
	/**
	 * @brief Load the leaderboards save game to this subsystem
	 * @return True if the leaderboards were loaded successfully
	 */
	UFUNCTION(BlueprintCallable, Category="Game Save", meta=(ReturnDisplayName="Save Was Loaded"))
	bool LoadLeaderboards(bool bOverwriteCurrent = false);
	UFUNCTION(BlueprintCallable, Category="Game Save")
	void SaveLeaderboards();

	UFUNCTION(BlueprintGetter, Category="Game Save")
	const TArray<FLeaderboardData>& GetLeaderboardData() const { return LeaderboardData; }
	
private:
	UPROPERTY(EditAnywhere, Category="Game Save")
	FString SaveSlotName = TEXT("Leaderboards");

	UPROPERTY(BLueprintGetter=GetLeaderboardData)
	TArray<FLeaderboardData> LeaderboardData;

	bool bLeaderboardsLoadAttempted = false;
};
