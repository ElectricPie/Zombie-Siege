// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LobbyHud.generated.h"

struct FWidgetControllerParams;
class UZSiegeUserWidget;
class ULobbyWidgetController;

/**
 * 
 */
UCLASS(Abstract)
class ZOMBIESIEGE_API ALobbyHud : public AHUD
{
	GENERATED_BODY()

public:
	ULobbyWidgetController* GetLobbyWidgetController(const FWidgetControllerParams& WidgetControllerParams);
	
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UZSiegeUserWidget> OverlayWidgetClass;
	UPROPERTY()
	TObjectPtr<UZSiegeUserWidget> OverlayWidget;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ULobbyWidgetController> LobbyWidgetControllerClass;
	UPROPERTY()
	TObjectPtr<ULobbyWidgetController> LobbyWidgetController;
};
