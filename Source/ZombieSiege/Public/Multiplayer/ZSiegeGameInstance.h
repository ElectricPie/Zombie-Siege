// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "ZSiegeGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESIEGE_API UZSiegeGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	
	UFUNCTION(BlueprintCallable)
	void HostGame() const;
	UFUNCTION(BlueprintCallable)
	void FindGames();
	UFUNCTION(BlueprintCallable)
	void JoinGame();
	
private:
	TWeakPtr<IOnlineSession> SessionInterface;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;

private:
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful) const;
	void OnFindSessionsComplete(bool bWasSuccessful) const;
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result) const;
};
