// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieSiege/Public/Ui/WidgetControllers/OverlayWidgetController.h"

#include "Components/MoneyStoreComponent.h"
#include "States/DefenceGameState.h"
#include "States/DefencePlayerState.h"


void UOverlayWidgetController::BindCallbackToDependencies()
{
	ADefenceGameState* GameState = Cast<ADefenceGameState>(GetWorld()->GetGameState());
	check(GameState);
	GameState->OnRoundChangedEvent.AddLambda(
		[this](const int32 RoundNumber)
		{
			OnRoundChangedEvent.Broadcast(RoundNumber);
		}
	);
	
	const ADefencePlayerState* DefencePlayerState = Cast<ADefencePlayerState>(PlayerState);
	check(DefencePlayerState);
	UMoneyStoreComponent* MoneyStoreComponent = IMoneyStoreInterface::Execute_GetMoneyStoreComponent(DefencePlayerState);
	MoneyStoreComponent->OnMoneyChangedEvent.AddLambda(
		[this](const int32 NewMoneyAmount, const int32 AmountChanged)
		{
			OnMoneyChangedEvent.Broadcast(NewMoneyAmount, AmountChanged);
		}
	);
}

void UOverlayWidgetController::BroadcastInitialValues()
{
	const ADefenceGameState* GameState = Cast<ADefenceGameState>(GetWorld()->GetGameState());
	check(GameState);
	OnRoundChangedEvent.Broadcast(GameState->GetCurrentRound());

	const ADefencePlayerState* DefencePlayerState = Cast<ADefencePlayerState>(PlayerState);
	check(DefencePlayerState);
	const UMoneyStoreComponent* MoneyStoreComponent = IMoneyStoreInterface::Execute_GetMoneyStoreComponent(DefencePlayerState);
	OnMoneyChangedEvent.Broadcast(MoneyStoreComponent->GetMoney(), 0);
}
