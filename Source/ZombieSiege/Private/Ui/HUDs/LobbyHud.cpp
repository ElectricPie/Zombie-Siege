// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/HUDs/LobbyHud.h"

#include "GameFramework/PlayerState.h"
#include "Ui/WidgetControllers/LobbyWidgetController.h"
#include "Ui/Widgets/ZSiegeUserWidget.h"

ULobbyWidgetController* ALobbyHud::GetLobbyWidgetController(const FWidgetControllerParams& WidgetControllerParams)
{
	if (LobbyWidgetController == nullptr)
	{
		LobbyWidgetController = NewObject<ULobbyWidgetController>(this, LobbyWidgetControllerClass);
		LobbyWidgetController->SetWidgetControllerParams(WidgetControllerParams);
		LobbyWidgetController->BindCallbackToDependencies();
	}

	return LobbyWidgetController;
}

void ALobbyHud::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PlayerController = GetOwningPlayerController();
	PlayerController->bShowMouseCursor = true;
	APlayerState* PlayerState = PlayerController->GetPlayerState<APlayerState>();
	const FWidgetControllerParams WidgetControllerParams(PlayerController, PlayerState);
	
	check(OverlayWidgetClass);
	OverlayWidget = CreateWidget<UZSiegeUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget->SetWidgetController(GetLobbyWidgetController(WidgetControllerParams));
	OverlayWidget->AddToViewport();
}
