// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemodes/LobbyGameMode.h"

#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// Kicks the player if the session is full
	if (PlayerControllers.Num() + 1 > MaxPlayers)
	{
		UE_LOG(LogTemp, Warning, TEXT("Session Full"));
		NewPlayer->ClientTravel("MainMenuNight", ETravelType::TRAVEL_Absolute);
		return;
	}

	PlayerControllers.Add(NewPlayer);
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	if (APlayerController* PlayerController = Cast<APlayerController>(Exiting))
	{
		PlayerControllers.Remove(PlayerController);
	}
}

AActor* ALobbyGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	if (SpawnPoints.Num() == 0)
	{
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), SpawnPoints);

		SpawnPoints.Sort([](const AActor& A, const AActor& B)
		{
			return A.GetName() < B.GetName();
		});
	}

	if (PlayerControllers.Num() < SpawnPoints.Num())
	{
		return SpawnPoints[PlayerControllers.Num()];
	}

	return Super::ChoosePlayerStart_Implementation(Player);
}
