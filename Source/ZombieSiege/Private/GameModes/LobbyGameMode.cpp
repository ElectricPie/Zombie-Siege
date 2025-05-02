// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemodes/LobbyGameMode.h"

#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerState.h"
#include "Gamemodes/GameSettingsDataAsset.h"
#include "Kismet/GameplayStatics.h"
#include "Lobby/LobbyPawn.h"
#include "Multiplayer/ZSiegeGameInstance.h"
#include "Player/PlayerCharacter.h"

void ALobbyGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	check(GameSettingsDataAsset);
	
	GameInstance = GetGameInstance<UZSiegeGameInstance>();
	check(GameInstance);
	GameInstance->InitMultiplayerGame(GameSettingsDataAsset->GetMaxPlayers());
}

APlayerController* ALobbyGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal,
	const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	GameInstance->AddMultiplayerPlayer(UniqueId->ToString());
	
	APlayerController* NewPlayerController = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);

	// Kicks the player if the session is full
	if (GameSettingsDataAsset->GetMaxPlayers() - GameInstance->GetPlayerCount() == 0)
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
			if (const FConnectedPlayerInfo* PlayerInfo = GameInstance->GetConnectedPlayerInfoByUniqueId_Server(PlayerState->GetUniqueId()->ToString()))
			{
				const int32 SpawnIndex = PlayerInfo->PlayerIndex;
				if (SpawnPoints.IsValidIndex(SpawnIndex))
				{
					return SpawnPoints[SpawnIndex];
				}
			}
		}
	}

	return Super::ChoosePlayerStart_Implementation(Player);
}

void ALobbyGameMode::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);

	if (ALobbyPawn* LobbyPawn = Cast<ALobbyPawn>(NewPlayer->GetPawn()))
	{
		const FConnectedPlayerInfo* PlayerInfo = GameInstance->GetConnectedPlayerInfoByUniqueId_Server(NewPlayer->PlayerState->GetUniqueId()->ToString());

		const FPlayerCharacterSkin& PlayerCharacterSkin = GameSettingsDataAsset->GetPlayerStartMeshes()[PlayerInfo->PlayerIndex];
		LobbyPawn->SetDesiredCharacterMesh_Server(PlayerCharacterSkin.CharacterMesh);
		if (PlayerCharacterSkin.CharacterHeadGearMesh)
		{
			LobbyPawn->SetDesiredHeadGearMesh_Server(PlayerCharacterSkin.CharacterHeadGearMesh);
		}
	}
}
