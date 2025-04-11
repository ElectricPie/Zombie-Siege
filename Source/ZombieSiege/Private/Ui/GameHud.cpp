// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/GameHud.h"

#include "Blueprint/UserWidget.h"
#include "Player/TopDownPlayerController.h"
#include "States/DefencePlayerState.h"
#include "Ui/WidgetControllers/AmmoCounterWidgetController.h"
#include "Ui/WidgetControllers/InteractionWidgetController.h"
#include "Widgets/OptionsWidget.h"
#include "ZombieSiege/Public/Ui/WidgetControllers/OverlayWidgetController.h"
#include "ZombieSiege/Public/Ui/Widgets/ZSiegeUserWidget.h"

void AGameHud::BeginPlay()
{
	Super::BeginPlay();

	// Setup Overlay
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class is null, please fill out in GameHud Blueprint"));
	OverlayWidget = CreateWidget<UZSiegeUserWidget>(GetWorld(), OverlayWidgetClass);
	Widgets.Add(OverlayWidget);

	APlayerController* PlayerController = GetOwningPlayerController();
	ADefencePlayerState* PlayerState = PlayerController->GetPlayerState<ADefencePlayerState>();
	const FWidgetControllerParams WidgetControllerParams(PlayerController, PlayerState);

	OverlayWidgetController = GetOverlayWidgetController(WidgetControllerParams);
	OverlayWidget->SetWidgetController(OverlayWidgetController);
	OverlayWidget->AddToViewport();
	OverlayWidgetController->BroadcastInitialValues();

	checkf(MenuWidgetClass, TEXT("Menu Widget Class is null, please fill out in GameHud Blueprint"));
	MenuWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), MenuWidgetClass);
	MenuWidget->AddToViewport();
	MenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	Widgets.Add(MenuWidget);

	checkf(OptionsWidgetClass, TEXT("Options Widget Class is null, please fill out in GameHud Blueprint"));
	OptionsWidget = CreateWidget<UOptionsWidget>(GetOwningPlayerController(), OptionsWidgetClass.Get());
	OptionsWidget->AddToViewport();
	OptionsWidget->SetVisibility(ESlateVisibility::Collapsed);
	OptionsWidget->OnOptionsClosedEvent.AddDynamic(this, &AGameHud::OnOptionsClosed);
	Widgets.Add(OptionsWidget);
}

void AGameHud::ShowGameOver()
{
	CollapseAllWidgets();

	if (GameOverWidgetClass)
	{
		GameOverWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), GameOverWidgetClass);
		GameOverWidget->AddToViewport();
	}
}


void AGameHud::SwitchActiveWidget(const EGameWidget WidgetToActivate)
{
	CollapseAllWidgets();

	switch (WidgetToActivate)
	{
	case EGameWidget::GameHud:
		if (OverlayWidget)
		{
			OverlayWidget->SetVisibility(ESlateVisibility::Visible);
			if (ATopDownPlayerController* PlayerController = Cast<
				ATopDownPlayerController>(GetOwningPlayerController()))
			{
				PlayerController->SetInputGameOnly();
			}
		}
		break;
	case EGameWidget::PauseMenu:
		if (MenuWidget)
		{
			MenuWidget->SetVisibility(ESlateVisibility::Visible);
			if (ATopDownPlayerController* PlayerController = Cast<
				ATopDownPlayerController>(GetOwningPlayerController()))
			{
				PlayerController->SetInputGameAndUI();
			}
		}
		break;
	case PauseGameOptions:
		if (OptionsWidget)
		{
			OptionsWidget->SetVisibility(ESlateVisibility::Visible);
			if (ATopDownPlayerController* PlayerController = Cast<
				ATopDownPlayerController>(GetOwningPlayerController()))
			{
				PlayerController->SetInputGameAndUI();
			}
		}
		break;
	default: ;
	}
}

void AGameHud::ToggleMenu()
{
	if (bMenuIsOpen)
	{
		SwitchActiveWidget(EGameWidget::GameHud);
		bMenuIsOpen = false;
	}
	else
	{
		SwitchActiveWidget(EGameWidget::PauseMenu);
		bMenuIsOpen = true;
	}

	OnPauseMenuToggledEvent.Broadcast(bMenuIsOpen);
}

UOverlayWidgetController* AGameHud::GetOverlayWidgetController(const FWidgetControllerParams& WidgetControllerParams)
{
	if (OverlayWidgetController == nullptr)
	{
		checkf(OverlayWidgetControllerClass,
		       TEXT("Overlay Widget Controller Class is null, please fill out in GameHud Blueprint"));
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WidgetControllerParams);
		OverlayWidgetController->BindCallbackToDependencies();
	}

	return OverlayWidgetController;
}

UAmmoCounterWidgetController* AGameHud::GetAmmoCounterWidgetController(
	const FWidgetControllerParams& WidgetControllerParams)
{
	if (AmmoCounterWidgetController == nullptr)
	{
		checkf(AmmoCounterWidgetControllerClass,
		       TEXT("Ammo Counter Widget Controller Class is null, please fill out in GameHud Blueprint"));
		AmmoCounterWidgetController = NewObject<UAmmoCounterWidgetController>(this, AmmoCounterWidgetControllerClass);
		AmmoCounterWidgetController->SetWidgetControllerParams(WidgetControllerParams);
		AmmoCounterWidgetController->BindCallbackToDependencies();
	}

	return AmmoCounterWidgetController;
}

UInteractionWidgetController* AGameHud::GetInteractionWidgetController(
	const FWidgetControllerParams& WidgetControllerParams)
{
	if (InteractionWidgetController == nullptr)
	{
		checkf(InteractionWidgetControllerClass,
		       TEXT("Interaction Widget Controller Class is null, please fill out in GameHud Blueprint"));
		InteractionWidgetController = NewObject<UInteractionWidgetController>(this, InteractionWidgetControllerClass);
		InteractionWidgetController->SetWidgetControllerParams(WidgetControllerParams);
		InteractionWidgetController->BindCallbackToDependencies();
	}

	return InteractionWidgetController;
}

UHealthWidgetController* AGameHud::GetHealthWidgetController(const FWidgetControllerParams& WidgetControllerParams)
{
	if (HealthWidgetController == nullptr)
	{
		checkf(HealthWidgetControllerClass,
		       TEXT("Health Widget Controller Class is null, please fill out in GameHud Blueprint"));
		HealthWidgetController = NewObject<UHealthWidgetController>(this, HealthWidgetControllerClass);
		HealthWidgetController->SetWidgetControllerParams(WidgetControllerParams);
		HealthWidgetController->BindCallbackToDependencies();
	}

	return HealthWidgetController;
}

void AGameHud::CollapseAllWidgets()
{
	for (const auto& Widget : Widgets)
	{
		if (Widget.IsValid())
		{
			Widget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void AGameHud::OnOptionsClosed()
{
	SwitchActiveWidget(EGameWidget::PauseMenu);
}
