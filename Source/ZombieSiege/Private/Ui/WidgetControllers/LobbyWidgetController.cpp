// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/WidgetControllers/LobbyWidgetController.h"

#include "Lobby/LobbyPlayerController.h"

void ULobbyWidgetController::BindCallbackToDependencies()
{
	ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(PlayerController);
	check(LobbyPlayerController);
	LobbyPlayerController->PlayerNamesUpdatedEvent.AddLambda([this](const TArray<FString>& PlayerNames)
	{
		for (int32 i = 0; i < PlayerNames.Num(); ++i)
		{
			OnPlayerNameChangedEvent.Broadcast(i, PlayerNames[i]);
		}
	});
}

void ULobbyWidgetController::BroadcastInitialValues()
{
	const ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(PlayerController);
	check(LobbyPlayerController)

	const TArray<FString>& PlayerNames = LobbyPlayerController->GetPlayerNames();
	for (int32 i = 0; i < PlayerNames.Num(); ++i)
	{
		OnPlayerNameChangedEvent.Broadcast(i, PlayerNames[i]);
	}
}
