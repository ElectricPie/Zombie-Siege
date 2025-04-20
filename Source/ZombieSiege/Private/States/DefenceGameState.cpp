// Fill out your copyright notice in the Description page of Project Settings.


#include "States/DefenceGameState.h"

#include "Net/UnrealNetwork.h"
#include "Player/PlayerCharacter.h"
#include "Player/TopDownPlayerController.h"

void ADefenceGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADefenceGameState, CurrentRound);
	DOREPLIFETIME(ADefenceGameState, AlivePlayerCharacters);
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
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Player->GetPawn()))
	{
		AlivePlayerCharacters.AddUnique(PlayerCharacter);
	}
}

void ADefenceGameState::RemoveAlivePlayer(ATopDownPlayerController* Player)
{
	check(Player);
	AlivePlayers.Remove(Player);
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Player->GetPawn()))
	{
		AlivePlayerCharacters.Remove(PlayerCharacter);
	}
	DeadPlayers.AddUnique(Player);
}

TArray<APlayerCharacter*> ADefenceGameState::GetAlivePlayerCharacters() const
{
	TArray<APlayerCharacter*> AliveCharacters;
	for (const auto& PlayerCharacter : AlivePlayerCharacters)
	{
		if (PlayerCharacter.Get())
		{
			AliveCharacters.AddUnique(PlayerCharacter.Get());
		}
	}

	return AliveCharacters;
}

void ADefenceGameState::RespawnPlayer(ATopDownPlayerController* Player)
{
	check(Player);

	DeadPlayers.Remove(Player);
	AlivePlayers.AddUnique(Player);
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Player->GetPawn()))
	{
		AlivePlayerCharacters.AddUnique(PlayerCharacter);
	}
}

void ADefenceGameState::OnRep_CurrentRound() const
{
	OnRoundChangedEvent.Broadcast(CurrentRound);
}
