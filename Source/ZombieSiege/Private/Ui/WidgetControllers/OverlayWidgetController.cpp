// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieSiege/Public/Ui/WidgetControllers/OverlayWidgetController.h"

#include "States/DefenceGameState.h"

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
}

void UOverlayWidgetController::BroadcastInitialValues()
{
	const ADefenceGameState* GameState = Cast<ADefenceGameState>(GetWorld()->GetGameState());
	check(GameState);
	OnRoundChangedEvent.Broadcast(GameState->GetCurrentRound());
}
