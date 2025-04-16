// Fill out your copyright notice in the Description page of Project Settings.


#include "States/DefencePlayerState.h"

#include "Money/MoneyStoreComponent.h"

ADefencePlayerState::ADefencePlayerState()
{
	MoneyStoreComponent = CreateDefaultSubobject<UMoneyStoreComponent>(TEXT("MoneyStoreComponent"));
}

UMoneyStoreComponent* ADefencePlayerState::GetMoneyStoreComponent_Implementation() const
{
	return MoneyStoreComponent;
}

void ADefencePlayerState::BeginPlay()
{
	Super::BeginPlay();

	MoneyStoreComponent->OnMoneyChangedEvent.AddLambda([this](const int32 NewMoneyAmount, const int32 AmountChanged)
	{
		const int32 ScoreToAdd = FMath::Max(0, AmountChanged);
		SetScore(GetScore() + ScoreToAdd);
	});
}
