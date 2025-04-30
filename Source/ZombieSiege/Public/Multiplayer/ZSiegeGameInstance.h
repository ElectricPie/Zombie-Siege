// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "ZSiegeGameInstance.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerNamesChangedSignature, const TArray<FString>&);

USTRUCT()
struct FConnectedPlayerInfo
{
	GENERATED_BODY()

public:
	FConnectedPlayerInfo()
	{
	}

	FConnectedPlayerInfo(const FString& InUniqueId, const int32 InPlayerIndex)
		: UniqueId(InUniqueId), PlayerIndex(InPlayerIndex)
	{
	}

	FString UniqueId;
	FString PlayerName = TEXT("None");
	int32 PlayerIndex = -1;
};

/**
 * 
 */
UCLASS()
class ZOMBIESIEGE_API UZSiegeGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FString LocalPlayerName = TEXT("");
	
	FOnPlayerNamesChangedSignature PlayerNamesChangedEvent;
	
public:
	virtual void Init() override;
	void InitMultiplayerGame(const int32 InMaxPlayers);
	
	UFUNCTION(BlueprintCallable)
	void HostGame() const;
	UFUNCTION(BlueprintCallable)
	void FindGames();
	UFUNCTION(BlueprintCallable)
	void JoinGame();
	UFUNCTION(BLueprintCallable, BlueprintPure=false)
	void EndSession() const;

	// Set the player name on the server
	void AddMultiplayerPlayer(const FString& UniqueId);
	void SetMultiplayerPlayerName_Server(FString UniqueId, const FString& NewPlayerName);
	void RemoveMultiplayerPlayer_Server(FString UniqueIdToRemove);

	int32 GetPlayerCount() const { return ConnectedPlayers.Num(); }

	FConnectedPlayerInfo* GetConnectedPlayerInfoByUniqueId_Server(const FString& UniqueId);

private:
	TWeakPtr<IOnlineSession> SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	
	// The player names in multiplayer games, only stored on host
	TArray<FConnectedPlayerInfo> ConnectedPlayers;
	int32 MaxPlayers = 4;
	TArray<int32> AvailablePlayerIndexes;
	
private:
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful) const;
	void OnFindSessionsComplete(bool bWasSuccessful) const;
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result) const;
	void HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString) const;

	void BroadcastPlayerNamesChanged() const;
};
