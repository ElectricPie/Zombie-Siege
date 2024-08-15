// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/MenuHud.h"

#include "Blueprint/UserWidget.h"
#include "Subsystems/GameSaveSubsystem.h"
#include "Widgets/OptionsWidget.h"

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
		case EMenuWidget::MainMenu:
			if (MenuWidget)
			{
				MenuWidget->SetVisibility(ESlateVisibility::Visible);
			}
			break;
		case EMenuWidget::MainOptions:
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
		OptionsWidget = CreateWidget<UOptionsWidget>(GetWorld(), OptionsWidgetClass.Get());
		if (OptionsWidget)
		{
			OptionsWidget->AddToViewport();
			OptionsWidget->SetVisibility(ESlateVisibility::Collapsed);
			OptionsWidget->OnOptionsClosedEvent.AddDynamic(this, &AMenuHud::OnOptionsClosed);
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

void AMenuHud::OnOptionsClosed()
{
	SwitchActiveWidget(EMenuWidget::MainMenu);
}
