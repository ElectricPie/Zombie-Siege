// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactions/ZSiegeInteractionSystemLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "States/DefencePlayerState.h"
#include "Ui/GameHud.h"
#include "Ui/WidgetControllers/InteractionWidgetController.h"

UInteractionWidgetController* UZSiegeInteractionSystemLibrary::GetInteractionWidgetController(
	const UObject* WorldContextObject)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AGameHud* GameHud = Cast<AGameHud>(PlayerController->GetHUD()))
		{
			ADefencePlayerState* PlayerState = PlayerController->GetPlayerState<ADefencePlayerState>();
			const FWidgetControllerParams WidgetControllerParams(PlayerController, PlayerState);
			return GameHud->GetInteractionWidgetController(WidgetControllerParams);
		}
	}

	return nullptr;
}