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

void AGameHud::SetInteractText(FText const& InteractText)
{
	if (!GameHudWidget) return;

	FText const Message = FText::Format(FText::FromString("Press E to {0}"), InteractText);
	GameHudWidget->UpdateInteractText(Message);
	GameHudWidget->ShowInteractText(true);
}

void AGameHud::HideInteractText()
{
	GameHudWidget->ShowInteractText(false);
}

