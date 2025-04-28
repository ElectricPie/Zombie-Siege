// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetController.h"
#include "LobbyWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerNameChanged, const int32, PlayerIndex, const FString, NewPlayerName);

/**
 * 
 */
UCLASS()
class ZOMBIESIEGE_API ULobbyWidgetController : public UWidgetController
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnPlayerNameChanged OnPlayerNameChangedEvent;

public:
	virtual void BindCallbackToDependencies() override;
	virtual void BroadcastInitialValues() override;
};
