// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/GameSaveSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "Leaderboards/LeaderboardsSaveGame.h"
#include "States/DefenceGameState.h"
#include "States/DefencePlayerState.h"

ULeaderboardsSaveGame* UGameSaveSubsystem::LoadLeaderboards()
{
	USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0);
	LeaderboardsSaveGame = Cast<ULeaderboardsSaveGame>(SaveGame);
	if (LeaderboardsSaveGame)
	{
		UE_LOG(LogTemp, Warning, TEXT("Save Found"));
	}

	return LeaderboardsSaveGame;
}

void UGameSaveSubsystem::SaveLeaderboards(ADefenceGameState* GameState, ADefencePlayerState* PlayerState)
{
	FLeaderboardData Data = {
		"None",
	    GameState->GetCurrentRound(),
	    PlayerState->GetTotalKills(),
	    FMath::FloorToInt32(PlayerState->GetScore()) ,
	    PlayerState->GetTotalDeaths(),
	    FDateTime::Now()
	};

	ULeaderboardsSaveGame::PrintLeaderboardData(Data);
}
