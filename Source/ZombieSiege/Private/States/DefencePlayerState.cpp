// Fill out your copyright notice in the Description page of Project Settings.


#include "States/DefencePlayerState.h"

void ADefencePlayerState::AddDeath()
{
	TotalDeaths++;
}

void ADefencePlayerState::AddKill()
{
	TotalKills++;
}
