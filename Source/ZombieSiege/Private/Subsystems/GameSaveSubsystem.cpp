// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/GameSaveSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "Leaderboards/LeaderboardsSaveGame.h"
#include "States/DefenceGameState.h"

void UGameSaveSubsystem::AddLeaderboardEntry(const FLeaderboardData& Data)
{
	LeaderboardData.Add(Data);
}

bool UGameSaveSubsystem::LoadLeaderboards(bool bOverwriteCurrent)
{
	// Ignore check if we are overwriting the current data
	if (!bOverwriteCurrent)
	{
		// Dont load if there are already entries
		if (LeaderboardData.Num() > 0)
			return false;
	}
	
	bLeaderboardsLoadAttempted = true;
	
	USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0);
	if (const ULeaderboardsSaveGame* LeaderboardsSaveGame = Cast<ULeaderboardsSaveGame>(SaveGame))
	{
		LeaderboardData = LeaderboardsSaveGame->LeaderboardData;
		return true;
	}

	return false;
}

void UGameSaveSubsystem::SaveLeaderboards()
{
	// if the leaderboards are empty try to load them
	if (!bLeaderboardsLoadAttempted)
	{
		// Save the current leaderboard data to be added back after loading
		const TArray<FLeaderboardData> TempLeaderboardData = LeaderboardData;
		UE_LOG(LogTemp, Warning, TEXT("SaveLeaderboards: have not been loaded, loading from save"));
		LoadLeaderboards(true);
		LeaderboardData.Append(TempLeaderboardData);
	}
	
	ULeaderboardsSaveGame* LeaderboardsSaveGame = Cast<ULeaderboardsSaveGame>(UGameplayStatics::CreateSaveGameObject(ULeaderboardsSaveGame::StaticClass()));
	if (LeaderboardsSaveGame == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create save game object"));
		return;
	}

	LeaderboardsSaveGame->LeaderboardData = LeaderboardData;
	if (UGameplayStatics::SaveGameToSlot(LeaderboardsSaveGame, SaveSlotName, 0))
	{
		UE_LOG(LogTemp, Warning, TEXT("Saved leaderboards"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to save leaderboards"));
	}
}
