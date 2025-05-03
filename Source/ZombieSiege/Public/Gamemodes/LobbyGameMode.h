// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LobbyGameMode.generated.h"

class UGameSettingsDataAsset;
class UZSiegeGameInstance;
/**
 * 
 */
UCLASS()
class ZOMBIESIEGE_API ALobbyGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	virtual void RestartPlayer(AController* NewPlayer) override;

private:
	UPROPERTY()
	TArray<AActor*> SpawnPoints;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UGameSettingsDataAsset> GameSettingsDataAsset;
	
	UPROPERTY()
	TObjectPtr<UZSiegeGameInstance> GameInstance;
};
