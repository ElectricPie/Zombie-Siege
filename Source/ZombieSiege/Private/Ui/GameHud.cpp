// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/GameHud.h"

#include "Blueprint/UserWidget.h"
#include "Widgets/GameHudWidget.h"

void AGameHud::BeginPlay()
{
	Super::BeginPlay();

	if (GameHudWidgetClass)
	{
		GameHudWidget = CreateWidget<UGameHudWidget>(GetOwningPlayerController(), GameHudWidgetClass);
		GameHudWidget->AddToViewport();
	}
}
