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
