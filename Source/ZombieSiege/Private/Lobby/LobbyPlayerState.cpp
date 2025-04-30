// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyPlayerState.h"

#include "Multiplayer/ZSiegeGameInstance.h"

void ALobbyPlayerState::SetPlayerName(const FString& S)
{
	Super::SetPlayerName(S);
}

void ALobbyPlayerState::ServerSetPlayerName_Implementation(const FString& NewPlayerName)
{
	SetPlayerName(NewPlayerName);
 
 	UZSiegeGameInstance* GameInstance = GetGameInstance<UZSiegeGameInstance>();
 	check(GameInstance);
 	GameInstance->SetMultiplayerPlayerName_Server(GetUniqueId()->ToString(), NewPlayerName);
}
