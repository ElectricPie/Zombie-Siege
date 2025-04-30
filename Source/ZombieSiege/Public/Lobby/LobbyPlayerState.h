// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LobbyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESIEGE_API ALobbyPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	virtual void SetPlayerName(const FString& S) override;

	UFUNCTION(Server, Reliable)
	void ServerSetPlayerName(const FString& NewPlayerName);
};
