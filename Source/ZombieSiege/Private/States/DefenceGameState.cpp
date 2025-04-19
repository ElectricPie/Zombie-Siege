// Fill out your copyright notice in the Description page of Project Settings.


#include "States/DefenceGameState.h"

#include "Net/UnrealNetwork.h"

void ADefenceGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADefenceGameState, CurrentRound);
}

void ADefenceGameState::StartNextRound()
{
	CurrentRound++;
	OnRoundChangedEvent.Broadcast(CurrentRound);
}

void ADefenceGameState::AddAlivePlayer(ATopDownPlayerController* Player)
{
	check(Player);

	AlivePlayers.AddUnique(Player);
}

void ADefenceGameState::RemoveAlivePlayer(ATopDownPlayerController* Player)
{
	check(Player);
	AlivePlayers.Remove(Player);
	DeadPlayers.AddUnique(Player);
}

void ADefenceGameState::RespawnPlayer(ATopDownPlayerController* Player)
{
	check(Player);

	DeadPlayers.Remove(Player);
	AlivePlayers.AddUnique(Player);
}

void ADefenceGameState::OnRep_CurrentRound() const
{
	OnRoundChangedEvent.Broadcast(CurrentRound);
}
