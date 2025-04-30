// Fill out your copyright notice in the Description page of Project Settings.

#include "Multiplayer/ZSiegeGameInstance.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Kismet/GameplayStatics.h"

void UZSiegeGameInstance::Init()
{
	if (const IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld()))
	{
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			IOnlineSession* Session = SessionInterface.Pin().Get();
			Session->OnCreateSessionCompleteDelegates.AddUObject(this, &UZSiegeGameInstance::OnCreateSessionComplete);
			Session->OnFindSessionsCompleteDelegates.AddUObject(this, &UZSiegeGameInstance::OnFindSessionsComplete);
			Session->OnJoinSessionCompleteDelegates.AddUObject(this, &UZSiegeGameInstance::OnJoinSessionComplete);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Session interface is not valid"));
		}
	}

	GetEngine()->OnNetworkFailure().AddUObject(this, &UZSiegeGameInstance::HandleNetworkFailure);
}

void UZSiegeGameInstance::InitMultiplayerGame(const int32 MaxPlayers)
{
	FMath::Max(MaxPlayers, 1);

	ConnectedPlayers.Empty();

	AvailablePlayerIndexes.Empty();
	AvailablePlayerIndexes.SetNum(MaxPlayers);
	for (int32 i = 0; i < MaxPlayers; i++)
	{
		AvailablePlayerIndexes[i] = i;
	}
}

void UZSiegeGameInstance::HostGame() const
{
	if (!SessionInterface.IsValid())
		return;

	IOnlineSession* Session = SessionInterface.Pin().Get();

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsLANMatch = true;
	SessionSettings.NumPublicConnections = 1;
	SessionSettings.bShouldAdvertise = true;

	Session->CreateSession(0, TEXT("Test Session"), SessionSettings);
}

void UZSiegeGameInstance::FindGames()
{
	if (!SessionInterface.IsValid())
		return;

	IOnlineSession* Session = SessionInterface.Pin().Get();

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = true;
	SessionSearch->MaxSearchResults = 10;

	Session->FindSessions(0, SessionSearch.ToSharedRef());
}

void UZSiegeGameInstance::JoinGame()
{
}

void UZSiegeGameInstance::AddMultiplayerPlayer(const FString& UniqueId)
{
	const int32 PlayerIndex = AvailablePlayerIndexes[0];
	AvailablePlayerIndexes.RemoveAt(0);

	const FConnectedPlayerInfo PlayerInfo(UniqueId, PlayerIndex);
	ConnectedPlayers.Add(PlayerInfo);
}

void UZSiegeGameInstance::SetMultiplayerPlayerName_Server(FString UniqueId, const FString& NewPlayerName)
{
	FConnectedPlayerInfo* PlayerInfo = ConnectedPlayers.FindByPredicate([UniqueId](const FConnectedPlayerInfo& Info)
	{
		return Info.UniqueId == UniqueId;
	});
	if (PlayerInfo)
	{
		PlayerInfo->PlayerName = NewPlayerName;
	}
}

void UZSiegeGameInstance::RemoveMultiplayerPlayer_Server(FString UniqueIdToRemove)
{
	const int32 Index = ConnectedPlayers.IndexOfByPredicate([UniqueIdToRemove](const FConnectedPlayerInfo& PlayerInfo)
	{
		return PlayerInfo.UniqueId == UniqueIdToRemove;
	});
	
	if (!ConnectedPlayers.IsValidIndex(Index))
	{
		return;
	}
	
	const FConnectedPlayerInfo* PlayerInfo = &ConnectedPlayers[Index];
	// Return the player index to the available pool
	AvailablePlayerIndexes.Add(PlayerInfo->PlayerIndex);
	// Keep new players index at the beginning of the array
	AvailablePlayerIndexes.Sort([](const int32 A, const int32 B)
	{
		return A < B;
	});
	
	ConnectedPlayers.RemoveAt(Index);

	BroadcastPlayerNamesChanged();
}

FConnectedPlayerInfo* UZSiegeGameInstance::GetConnectedPlayerInfoByUniqueId_Server(const FString& UniqueId)
{
	return ConnectedPlayers.FindByPredicate([UniqueId](const FConnectedPlayerInfo& Info)
	{
		return Info.UniqueId == UniqueId;;
	});
}


void UZSiegeGameInstance::EndSession() const
{
	if (!SessionInterface.IsValid())
		return;

	IOnlineSession* Session = SessionInterface.Pin().Get();
	Session->DestroySession(TEXT("Test Session"));
}

void UZSiegeGameInstance::OnCreateSessionComplete(FName SessionName, const bool bWasSuccessful) const
{
	if (bWasSuccessful)
	{
		UGameplayStatics::OpenLevel(this, "TrainingGroundsNightLobby", true, "listen");
	}
}

void UZSiegeGameInstance::OnFindSessionsComplete(const bool bWasSuccessful) const
{
	if (bWasSuccessful && SessionSearch.IsValid())
	{
		for (const FOnlineSessionSearchResult& Result : SessionSearch->SearchResults)
		{
			// TODO: Joining first for testing
			SessionInterface.Pin()->JoinSession(0, TEXT("Test Session"), Result);

			return;
		}
	}
}

void UZSiegeGameInstance::OnJoinSessionComplete(const FName SessionName,
                                                EOnJoinSessionCompleteResult::Type Result) const
{
	if (SessionInterface.IsValid())
	{
		FString ConnectString;
		SessionInterface.Pin()->GetResolvedConnectString(SessionName, ConnectString);
		if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
		{
			PlayerController->ClientTravel(ConnectString, ETravelType::TRAVEL_Absolute);
		}
	}
}

void UZSiegeGameInstance::HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType,
                                               const FString& ErrorString) const
{
	if (FailureType == ENetworkFailure::ConnectionLost || FailureType == ENetworkFailure::ConnectionTimeout)
	{
		UKismetSystemLibrary::PrintString(World, TEXT("Connection lost or timed out. Returning to main menu."), true,
		                                  true, FLinearColor::Red, 5.f);
		UGameplayStatics::OpenLevel(this, "MainMenuNight");
	}
}

void UZSiegeGameInstance::BroadcastPlayerNamesChanged() const
{
	TArray<FString> PlayerNames;
	PlayerNames.SetNum(ConnectedPlayers.Num());
	for (const auto& Player : ConnectedPlayers)
	{
		const int32 ConnectedPlayerIndex = Player.PlayerIndex;
		if (PlayerNames.IsValidIndex(ConnectedPlayerIndex))
		{
			PlayerNames[ConnectedPlayerIndex] = Player.PlayerName;
		}
		UE_LOG(LogTemp, Warning, TEXT("Broadcasting %d to %s"), ConnectedPlayerIndex, *Player.PlayerName);
	}

	PlayerNamesChangedEvent.Broadcast(PlayerNames);
}
