// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemodes/LobbyGameMode.h"

#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	PlayerCount++;
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

	if (PlayerCount < SpawnPoints.Num())
	{
		return SpawnPoints[PlayerCount];
	}

	return Super::ChoosePlayerStart_Implementation(Player);
}
