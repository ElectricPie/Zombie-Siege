// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LobbyGameMode.generated.h"

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
	virtual void Logout(AController* Exiting) override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

private:
	UPROPERTY()
	TArray<AActor*> SpawnPoints;

	UPROPERTY(EditAnywhere)
	int32 MaxPlayers = 4;
	
	UPROPERTY()
	TObjectPtr<UZSiegeGameInstance> GameInstance;
};
