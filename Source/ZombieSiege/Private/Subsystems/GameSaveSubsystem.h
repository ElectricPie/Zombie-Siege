// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameSaveSubsystem.generated.h"

class ULeaderboardsSaveGame;
/**
 * 
 */
UCLASS()
class UGameSaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/**
	 * @brief Load the leaderboards save game to this subsystem
	 * @return The leaderboards save game, nullptr if not found
	 */
	UFUNCTION(BlueprintCallable, Category="Game Save", meta=(ReturnDisplayName="Save Game"))
	ULeaderboardsSaveGame* LoadLeaderboards();
	UFUNCTION(BlueprintCallable, Category="Game Save")
	void SaveLeaderboards(ADefenceGameState* GameState, ADefencePlayerState* PlayerState);
	
private:
	UPROPERTY(EditAnywhere, Category="Game Save")
	FString SaveSlotName = TEXT("Leaderboards");
	
	TObjectPtr<ULeaderboardsSaveGame> LeaderboardsSaveGame = nullptr;
};
