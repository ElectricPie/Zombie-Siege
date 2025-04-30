// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemodes/LobbyGameMode.h"

#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Multiplayer/ZSiegeGameInstance.h"

void ALobbyGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	GameInstance = GetGameInstance<UZSiegeGameInstance>();
	check(GameInstance);
	GameInstance->InitMultiplayerGame(MaxPlayers);
}

APlayerController* ALobbyGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal,
	const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	GameInstance->AddMultiplayerPlayer(UniqueId->ToString());
	
	APlayerController* NewPlayerController = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);

	// Kicks the player if the session is full
	if (MaxPlayers - GameInstance->GetPlayerCount() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Session Full"));
		NewPlayerController->ClientTravel("MainMenuNight", ETravelType::TRAVEL_Absolute);
		return NewPlayerController;
	}
	
	return NewPlayerController;
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	if (const APlayerController* PlayerController = Cast<APlayerController>(Exiting))
	{
		if (const APlayerState* PlayerState = PlayerController->PlayerState)
		{
			GameInstance->RemoveMultiplayerPlayer_Server(PlayerState->GetUniqueId()->ToString());
		}
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

	if (const APlayerController* PlayerController = Cast<APlayerController>(Player))
	{
		if (const APlayerState* PlayerState = PlayerController->PlayerState)
		{
			if (PlayerState->GetUniqueId() == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("UniqueId is nullptr"));
				return Super::ChoosePlayerStart_Implementation(Player);
			}
			
			if (const FConnectedPlayerInfo* PlayerInfo = GameInstance->GetConnectedPlayerInfoByUniqueId_Server(PlayerState->GetUniqueId()->ToString()))
			{
				const int32 SpawnIndex = PlayerInfo->PlayerIndex;
				UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("SpawnIndex: %d"), SpawnIndex), true, true, FLinearColor::Red, 10.0f);
				if (SpawnPoints.IsValidIndex(SpawnIndex))
				{
					return SpawnPoints[SpawnIndex];
				}
			}
		}
	}

	return Super::ChoosePlayerStart_Implementation(Player);
}
