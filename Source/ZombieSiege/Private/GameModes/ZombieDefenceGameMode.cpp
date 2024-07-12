// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/ZombieDefenceGameMode.h"

#include "Player/TopDownPlayerController.h"

void AZombieDefenceGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	if (ATopDownPlayerController* Player = Cast<ATopDownPlayerController>(NewPlayer))
	{
		Player->SetMoney(StartingMoney);
	}
}
