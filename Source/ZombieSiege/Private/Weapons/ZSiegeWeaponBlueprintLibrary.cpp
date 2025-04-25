// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/ZSiegeWeaponBlueprintLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "States/DefencePlayerState.h"
#include "Ui/HUDs/GameHud.h"
#include "Ui/WidgetControllers/WidgetController.h"

UAmmoCounterWidgetController* UZSiegeWeaponBlueprintLibrary::GetAmmoCounterWidgetController(
	const UObject* WorldContextObject)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AGameHud* GameHud = Cast<AGameHud>(PlayerController->GetHUD()))
		{
			ADefencePlayerState* PlayerState = PlayerController->GetPlayerState<ADefencePlayerState>();
			const FWidgetControllerParams WidgetControllerParams(PlayerController, PlayerState);
			return GameHud->GetAmmoCounterWidgetController(WidgetControllerParams);
		}
	}

	return nullptr;
}
