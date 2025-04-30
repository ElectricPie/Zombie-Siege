// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyPlayerController.h"

#include "GameFramework/PlayerState.h"
#include "Lobby/LobbyPlayerState.h"
#include "Multiplayer/ZSiegeGameInstance.h"
#include "Net/UnrealNetwork.h"


class ALobbyPlayerState;

void ALobbyPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Set the player name for the clients
	ALobbyPlayerState* LobbyPlayerState = Cast<ALobbyPlayerState>(PlayerState);
	check(LobbyPlayerState);
	const UZSiegeGameInstance* GameInstance = GetGameInstance<UZSiegeGameInstance>();
	check(GameInstance);
	LobbyPlayerState->ServerSetPlayerName(GameInstance->LocalPlayerName);
}

void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		UZSiegeGameInstance* GameInstance = GetGameInstance<UZSiegeGameInstance>();
		check(GameInstance);
		GameInstance->PlayerNamesChangedEvent.AddUObject(this, &ALobbyPlayerController::OnPlayerNamesUpdated);

		if (IsLocalController())
		{
			// Set the player name for the listen host
			ALobbyPlayerState* LobbyPlayerState = Cast<ALobbyPlayerState>(PlayerState);
			check(LobbyPlayerState);
			LobbyPlayerState->ServerSetPlayerName(GameInstance->LocalPlayerName);
		}
	}
}

void ALobbyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPlayerController, PlayerNames);
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
