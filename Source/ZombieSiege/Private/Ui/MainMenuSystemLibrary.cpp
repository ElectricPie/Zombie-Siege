// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/MainMenuSystemLibrary.h"

#include "MenuHud.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Ui/WidgetControllers/WidgetController.h"

UMainMenuWidgetController* UMainMenuSystemLibrary::GetMainMenuWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AMenuHud* MenuHud = PlayerController->GetHUD<AMenuHud>())
		{
			const FWidgetControllerParams WidgetControllerParams(PlayerController, PlayerController->GetPlayerState<APlayerState>());
			return MenuHud->GetMainMenuWidgetController(WidgetControllerParams);
		}
	}
	
	
	return nullptr;
}
