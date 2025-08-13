// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/GameSaveSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "Leaderboards/LeaderboardsSaveGame.h"
#include "Multiplayer/ZSiegeGameInstance.h"

void UGameSaveSubsystem::AddLeaderboardEntry(const FLeaderboardData& Data)
{
	const int32 InsertIndex = LeaderboardData.IndexOfByPredicate([&Data](const FLeaderboardData& LeaderboardData)
	{
		return LeaderboardData.RoundsSurvived < Data.RoundsSurvived;
	});

	if (InsertIndex == INDEX_NONE)
	{
		LeaderboardData.Add(Data);
	}
	else
	{
		LeaderboardData.Insert(Data, InsertIndex);
	}
}

bool UGameSaveSubsystem::Load(const bool bOverwriteCurrent)
{
	// Ignore check if we are overwriting the current data
	if (!bOverwriteCurrent)
	{
		// Don't load if there are already entries
		if (LeaderboardData.Num() > 0)
			return false;
	}
	
	bLeaderboardsLoadAttempted = true;
	
	USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(SaveGameSlotName, 0);
	if (const UZSiegeSaveGame* ZSiegeSaveGame = Cast<UZSiegeSaveGame>(SaveGame))
	{
		LeaderboardData = ZSiegeSaveGame->LeaderboardData;
		Cast<UZSiegeGameInstance>(GetGameInstance())->LocalPlayerName = ZSiegeSaveGame->PlayerName;
		OnGameLoaded.Broadcast();
		return true;
	}

	return false;
}

void UGameSaveSubsystem::Save()
{
	// if the leaderboards are empty try to load them
	if (!bLeaderboardsLoadAttempted)
	{
		// Save the current leaderboard data to be added back after loading
		const TArray<FLeaderboardData> TempLeaderboardData = LeaderboardData;
		UE_LOG(LogTemp, Warning, TEXT("SaveLeaderboards: have not been loaded, loading from save"));
		Load(true);
		for (auto& Data : TempLeaderboardData)
		{
			AddLeaderboardEntry(Data);
		}
	}

	UZSiegeSaveGame* LeaderboardsSaveGame = Cast<UZSiegeSaveGame>(UGameplayStatics::CreateSaveGameObject(UZSiegeSaveGame::StaticClass()));
	LeaderboardsSaveGame->PlayerName = Cast<UZSiegeGameInstance>(GetGameInstance())->LocalPlayerName;

	if (LeaderboardsSaveGame == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create save game object"));
		return;
	}

	LeaderboardsSaveGame->LeaderboardData = LeaderboardData;
	if (UGameplayStatics::SaveGameToSlot(LeaderboardsSaveGame, SaveGameSlotName, 0))
	{
		UE_LOG(LogTemp, Warning, TEXT("Saved game data"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to save game data"));
	}
}