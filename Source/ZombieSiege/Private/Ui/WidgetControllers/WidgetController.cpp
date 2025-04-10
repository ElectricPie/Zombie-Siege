// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieSiege/Public/Ui/WidgetControllers/WidgetController.h"

void UWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& InParams)
{
	PlayerController = InParams.PlayerController;
}

void UWidgetController::BindCallbackToDependencies()
{
}

void UWidgetController::BroadcastInitialValues()
{
}
