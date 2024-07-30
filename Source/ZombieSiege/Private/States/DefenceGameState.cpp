// Fill out your copyright notice in the Description page of Project Settings.


#include "States/DefenceGameState.h"

void ADefenceGameState::StartNextRound()
{
	CurrentRound++;
	OnRoundChangedEvent.Broadcast(CurrentRound);
}
