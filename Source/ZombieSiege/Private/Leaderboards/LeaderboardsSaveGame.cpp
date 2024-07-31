// Fill out your copyright notice in the Description page of Project Settings.


#include "Leaderboards/LeaderboardsSaveGame.h"

void ULeaderboardsSaveGame::PrintLeaderboardData(FLeaderboardData Data)
{
	UE_LOG(LogTemp, Warning, TEXT("PlayerName: %s"), *Data.PlayerName);
	UE_LOG(LogTemp, Warning, TEXT("RoundsSurvived: %d"), Data.RoundsSurvived);
	UE_LOG(LogTemp, Warning, TEXT("Kills: %d"), Data.Kills);
	UE_LOG(LogTemp, Warning, TEXT("Score: %d"), Data.Score);
	UE_LOG(LogTemp, Warning, TEXT("Deaths: %d"), Data.Deaths);
	UE_LOG(LogTemp, Warning, TEXT("DateAchieved: %s"), *Data.DateAchieved.ToString());
}