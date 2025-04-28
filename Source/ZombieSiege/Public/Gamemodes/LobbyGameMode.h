// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESIEGE_API ALobbyGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

private:
	UPROPERTY()
	TArray<AActor*> SpawnPoints;

	UPROPERTY(EditAnywhere)
	int32 MaxPlayers = 4;

	UPROPERTY()
	TArray<TObjectPtr<APlayerController>> PlayerControllers;
};
