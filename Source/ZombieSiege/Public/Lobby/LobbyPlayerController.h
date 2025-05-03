// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPlayerController.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerNamesUpdatedSignature, const TArray<FString>&);

/**
 * 
 */
UCLASS()
class ZOMBIESIEGE_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	FOnPlayerNamesUpdatedSignature PlayerNamesUpdatedEvent;

public:
	const TArray<FString>& GetPlayerNames() const { return PlayerNames; }
	
	virtual void OnRep_PlayerState() override;
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(ReplicatedUsing=OnRep_PlayerNames)
	TArray<FString> PlayerNames;
	
private:
	void OnPlayerNamesUpdated(const TArray<FString>& NewPlayerNames);
	UFUNCTION()
	void OnRep_PlayerNames() const;


};
