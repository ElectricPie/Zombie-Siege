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

void ADefenceGameState::AddAlivePlayer(AController* Player)
{
	check(Player);

	AlivePlayers.AddUnique(Player);
}

void ADefenceGameState::RemoveAlivePlayer(AController* Player)
{
	check(Player);
	AlivePlayers.Remove(Player);
}

void ADefenceGameState::OnRep_CurrentRound() const
{
	OnRoundChangedEvent.Broadcast(CurrentRound);
}
