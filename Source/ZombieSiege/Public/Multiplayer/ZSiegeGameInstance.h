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
	FConnectedPlayerInfo() : PlayerId(0), PlayerName(TEXT("")) {}
	FConnectedPlayerInfo(const int32 InPlayerId, const FString& InPlayerName)
		: PlayerId(InPlayerId), PlayerName(InPlayerName) {}
	
	int32 PlayerId;
	FString PlayerName;
};

/**
 * 
 */
UCLASS()
class ZOMBIESIEGE_API UZSiegeGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	FOnPlayerNamesChangedSignature PlayerNamesChangedEvent;
	
public:
	virtual void Init() override;
	
	UFUNCTION(BlueprintCallable)
	void HostGame() const;
	UFUNCTION(BlueprintCallable)
	void FindGames();
	UFUNCTION(BlueprintCallable)
	void JoinGame();
	UPROPERTY(BlueprintReadWrite)
	FString LocalPlayerName = TEXT("");

	// Set the player name on the server
	void SetMultiplayerPlayerName(int32 PlayerId, const FString& NewPlayerName);
	
	// The player names in multiplayer games, only stored on host
	TArray<FConnectedPlayerInfo> ConnectedPlayers;

private:
	TWeakPtr<IOnlineSession> SessionInterface;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	int32 ConnectedPlayersCount = 0;
	
private:
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful) const;
	void OnFindSessionsComplete(bool bWasSuccessful) const;
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result) const;
};
