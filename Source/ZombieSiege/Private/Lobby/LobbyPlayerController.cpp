// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyPlayerController.h"

#include "GameFramework/PlayerState.h"
#include "Multiplayer/ZSiegeGameInstance.h"
#include "Net/UnrealNetwork.h"

void ALobbyPlayerController::ServerSetPlayerName_Implementation(const FString& NewPlayerName)
{
	UZSiegeGameInstance* GameInstance = GetGameInstance<UZSiegeGameInstance>();
	check(GameInstance);
	GameInstance->SetMultiplayerPlayerName(PlayerState->GetPlayerId(), NewPlayerName);
}

void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		UZSiegeGameInstance* GameInstance = GetGameInstance<UZSiegeGameInstance>();
		check(GameInstance);
		GameInstance->PlayerNamesChangedEvent.AddLambda([this](const TArray<FString>& NewPlayerNames)
		{
			PlayerNames = NewPlayerNames;
			PlayerNamesUpdatedEvent.Broadcast(PlayerNames);
		});
	}
	
	if (IsLocalController())
	{
		const UZSiegeGameInstance* GameInstance = GetGameInstance<UZSiegeGameInstance>();
		check(GameInstance);
		ServerSetPlayerName(GameInstance->LocalPlayerName);
	}
}

void ALobbyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPlayerController, PlayerNames);
}

void ALobbyPlayerController::OnRep_PlayerNames() const
{
	PlayerNamesUpdatedEvent.Broadcast(PlayerNames);
}
