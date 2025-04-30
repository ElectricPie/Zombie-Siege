// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyPlayerController.h"

#include "GameFramework/PlayerState.h"
#include "Multiplayer/ZSiegeGameInstance.h"
#include "Net/UnrealNetwork.h"


void ALobbyPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Set the player name for the clients
	const UZSiegeGameInstance* GameInstance = GetGameInstance<UZSiegeGameInstance>();
	check(GameInstance);
	ServerSetPlayerName(GameInstance->LocalPlayerName);
}

void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		UZSiegeGameInstance* GameInstance = GetGameInstance<UZSiegeGameInstance>();
		check(GameInstance);
		GameInstance->PlayerNamesChangedEvent.AddUObject(this, &ALobbyPlayerController::OnPlayerNamesUpdated);
	}
}

void ALobbyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPlayerController, PlayerNames);
}

void ALobbyPlayerController::InitPlayerState()
{
	Super::InitPlayerState();

	// Set the player name for the listen host
	const UZSiegeGameInstance* GameInstance = GetGameInstance<UZSiegeGameInstance>();
	check(GameInstance);
	ServerSetPlayerName(GameInstance->LocalPlayerName);
}

void ALobbyPlayerController::ServerSetPlayerName_Implementation(const FString& NewPlayerName) const
{
	// Store the player name in the game instance
	UZSiegeGameInstance* GameInstance = GetGameInstance<UZSiegeGameInstance>();
	check(GameInstance);
	// GameInstance->SetMultiplayerPlayerName_Server(PlayerState->GetUniqueId()->ToString(), NewPlayerName);
}

void ALobbyPlayerController::OnPlayerNamesUpdated(const TArray<FString>& NewPlayerNames)
{
	PlayerNames = NewPlayerNames;
	PlayerNamesUpdatedEvent.Broadcast(PlayerNames);
}

void ALobbyPlayerController::OnRep_PlayerNames() const
{
	PlayerNamesUpdatedEvent.Broadcast(PlayerNames);
}

