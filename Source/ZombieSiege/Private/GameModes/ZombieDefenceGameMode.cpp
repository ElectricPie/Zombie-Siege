// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/ZombieDefenceGameMode.h"

#include "Components/MoneyStoreComponent.h"

void AZombieDefenceGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	if (UMoneyStoreComponent* MoneyStore = NewPlayer->GetComponentByClass<UMoneyStoreComponent>())
	{
		MoneyStore->SetMoney(StartingMoney);
	}
}
