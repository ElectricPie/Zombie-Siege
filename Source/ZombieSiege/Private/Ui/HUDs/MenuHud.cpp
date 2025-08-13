// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/HUDs/MenuHud.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerState.h"
#include "Subsystems/GameSaveSubsystem.h"
#include "Ui/WidgetControllers/MainMenuWidgetController.h"
#include "Ui/Widgets/ZSiegeUserWidget.h"
#include "Ui/Widgets/OptionsWidget.h"

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

UMainMenuWidgetController* AMenuHud::GetMainMenuWidgetController(const FWidgetControllerParams& WidgetControllerParams)
{
	if (MainMenuWidgetController == nullptr)
	{
		checkf(MainMenuWidgetControllerClass,
		       TEXT("Main Menu Widget Controller Class is null, please fill out in MenuHud Blueprint"));
		MainMenuWidgetController = NewObject<UMainMenuWidgetController>(this, MainMenuWidgetControllerClass);
		MainMenuWidgetController->SetWidgetControllerParams(WidgetControllerParams);
		MainMenuWidgetController->BindCallbackToDependencies();
	}

	return MainMenuWidgetController;
}

void AMenuHud::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PlayerController = GetOwningPlayerController();
	PlayerController->bShowMouseCursor = true;
	APlayerState* PlayerState = PlayerController->GetPlayerState<APlayerState>();
	const FWidgetControllerParams WidgetControllerParams(PlayerController, PlayerState);
	
	check(MainMenuWidgetClass);
	MenuWidget = CreateWidget<UZSiegeUserWidget>(GetWorld(), MainMenuWidgetClass);
	MenuWidget->SetWidgetController(GetMainMenuWidgetController(WidgetControllerParams));
	MenuWidget->AddToViewport();
	Widgets.Add(MenuWidget);


	check(OptionsWidgetClass)
	OptionsWidget = CreateWidget<UOptionsWidget>(GetWorld(), OptionsWidgetClass.Get());
	OptionsWidget->AddToViewport();
	OptionsWidget->SetVisibility(ESlateVisibility::Collapsed);
	OptionsWidget->OnOptionsClosedEvent.AddDynamic(this, &AMenuHud::OnOptionsClosed);
	Widgets.Add(OptionsWidget);
}

void AMenuHud::OnOptionsClosed()
{
	SwitchActiveWidget(EMenuWidget::MainMenu);
}
