// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/HUDs/LobbyHud.h"

#include "Ui/Widgets/ZSiegeUserWidget.h"

void ALobbyHud::BeginPlay()
{
	Super::BeginPlay();

	check(OverlayWidgetClass);
	OverlayWidget = CreateWidget<UZSiegeUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget->AddToViewport();
}
