// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/MenuHud.h"

#include "Blueprint/UserWidget.h"
#include "Subsystems/GameSaveSubsystem.h"

void AMenuHud::SwitchActiveWidget(EMenuWidget WidgetToActivate)
{
	for (const auto& Widget : Widgets)
	{
		if (Widget.IsValid())
		{
			Widget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	switch (WidgetToActivate)
	{
		case MainMenu:
			if (MenuWidget)
			{
				MenuWidget->SetVisibility(ESlateVisibility::Visible);
			}
			break;
		case Options:
			if (OptionsWidget)
			{
				OptionsWidget->SetVisibility(ESlateVisibility::Visible);
			}
			break;
		default: ;
	}
}

void AMenuHud::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = GetOwningPlayerController())
	{
		PlayerController->bShowMouseCursor = true;
	}
	
	if (MainMenuWidgetClass)
	{
		MenuWidget = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass);
		if (MenuWidget)
		{
			MenuWidget->AddToViewport();
			Widgets.Add(MenuWidget);
		}
	}

	if (OptionsWidgetClass)
	{
		OptionsWidget = CreateWidget<UUserWidget>(GetWorld(), OptionsWidgetClass);
		if (OptionsWidget)
		{
			OptionsWidget->AddToViewport();
			OptionsWidget->SetVisibility(ESlateVisibility::Collapsed);
			Widgets.Add(OptionsWidget);
		}
	}

	if (const UGameInstance* GameInstance = GetGameInstance())
	{
		if (UGameSaveSubsystem* GameSaveSubsystem = GameInstance->GetSubsystem<UGameSaveSubsystem>())
		{
			GameSaveSubsystem->LoadLeaderboards();
		}
	}
}
