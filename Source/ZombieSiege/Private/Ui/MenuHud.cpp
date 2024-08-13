// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/MenuHud.h"

#include "Blueprint/UserWidget.h"
#include "Subsystems/GameSaveSubsystem.h"

//
// void AMenuHud::SwitchHud(EMenuWidget WidgetToDisplay)
// {
// }

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
		}
	}

	if (OptionsWidgetClass)
	{
		OptionsWidget = CreateWidget<UUserWidget>(GetWorld(), OptionsWidgetClass);
		if (OptionsWidget)
		{
			OptionsWidget->AddToViewport();
			OptionsWidget->SetVisibility(ESlateVisibility::Collapsed);
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
