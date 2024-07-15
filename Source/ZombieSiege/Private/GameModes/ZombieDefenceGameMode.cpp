// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/ZombieDefenceGameMode.h"

#include "Components/MoneyStoreComponent.h"

void AZombieDefenceGameMode::BeginPlay()
{
	Super::BeginPlay();

}

void AZombieDefenceGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	if (UMoneyStoreComponent* MoneyStore = NewPlayer->GetComponentByClass<UMoneyStoreComponent>())
	{
		MoneyStore->SetMoney(StartingMoney);
	}
}

int32 AZombieDefenceGameMode::WaveCountBelow20()
{
	return -1.091f + 6.312f * RoundNumber - 0.421f * (RoundNumber * RoundNumber) + 0.013 * (RoundNumber * RoundNumber * RoundNumber);
}

int32 AZombieDefenceGameMode::WaveCount20AndAbove()
{
	return 0.09f * (RoundNumber * RoundNumber) - 0.0029f * RoundNumber + 23.9580;
}
