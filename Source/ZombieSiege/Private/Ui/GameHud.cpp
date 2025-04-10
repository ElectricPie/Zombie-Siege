// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/GameHud.h"

#include "Blueprint/UserWidget.h"
#include "Player/PlayerCharacter.h"
#include "Player/TopDownPlayerController.h"
#include "States/DefenceGameState.h"
#include "Widgets/GameHudWidget.h"
#include "Widgets/OptionsWidget.h"
#include "ZombieSiege/Public/Ui/WidgetControllers/OverlayWidgetController.h"
#include "ZombieSiege/Public/Ui/Widgets/ZSiegeUserWidget.h"

void AGameHud::BeginPlay()
{
	Super::BeginPlay();

	// if (GameHudWidgetClass)
	// {
	// 	if (ATopDownPlayerController* PlayerController = Cast<ATopDownPlayerController>(GetOwningPlayerController()))
	// 	{
	// 		GameHudWidget = CreateWidget<UGameHudWidget>(PlayerController, GameHudWidgetClass);
	// 		GameHudWidget->AddToViewport();
	// 		Widgets.Add(GameHudWidget);
	// 		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetPawn()))
	// 		{
	// 			GameHudWidget->Setup(PlayerController, PlayerCharacter);
	// 		}
	// 	}
	// }

	// Setup Overlay
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class is null, please fill out in GameHud Blueprint"));
	OverlayWidget = CreateWidget<UZSiegeUserWidget>(GetWorld(), OverlayWidgetClass);
	const FWidgetControllerParams WidgetControllerParams(GetOwningPlayerController());
	OverlayWidgetController = GetOverlayWidgetController(WidgetControllerParams);
	OverlayWidgetController->BindCallbackToDependencies();
	OverlayWidget->SetWidgetController(OverlayWidgetController);
	OverlayWidget->AddToViewport();
	OverlayWidgetController->BroadcastInitialValues();
	
	if (MenuWidgetClass)
	{
		MenuWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), MenuWidgetClass);
		MenuWidget->AddToViewport();
		MenuWidget->SetVisibility(ESlateVisibility::Collapsed);
		Widgets.Add(MenuWidget);
	}

	if (OptionsWidgetClass)
	{
		OptionsWidget = CreateWidget<UOptionsWidget>(GetOwningPlayerController(), OptionsWidgetClass.Get());
		OptionsWidget->AddToViewport();
		OptionsWidget->SetVisibility(ESlateVisibility::Collapsed);
		OptionsWidget->OnOptionsClosedEvent.AddDynamic(this, &AGameHud::OnOptionsClosed);
		Widgets.Add(OptionsWidget);
	}

	if (ADefenceGameState* GameState = GetWorld()->GetGameState<ADefenceGameState>())
	{
		GameState->OnRoundChangedEvent.AddUObject(this, &AGameHud::OnRoundChanged);
	}
}

void AGameHud::SetInteractText(FText const& InteractText)
{
	// if (!GameHudWidget) return;
	//
	// FText const Message = FText::Format(FText::FromString("Press E to {0}"), InteractText);
	// GameHudWidget->UpdateInteractText(Message);
	// GameHudWidget->ShowInteractText(true);
}

void AGameHud::HideInteractText()
{
	// GameHudWidget->ShowInteractText(false);
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
		// if (GameHudWidget)
		// {
		// 	GameHudWidget->SetVisibility(ESlateVisibility::Visible);
		// 	if (ATopDownPlayerController* PlayerController = Cast<ATopDownPlayerController>(GetOwningPlayerController()))
		// 	{
		// 		PlayerController->SetInputGameOnly();
		// 	}
		// }
		break;
	case EGameWidget::PauseMenu:
		if (MenuWidget)
		{
			MenuWidget->SetVisibility(ESlateVisibility::Visible);
			if (ATopDownPlayerController* PlayerController = Cast<ATopDownPlayerController>(GetOwningPlayerController()))
			{
				PlayerController->SetInputGameAndUI();
			}
		}
		break;
	case PauseGameOptions:
		if (OptionsWidget)
		{
			OptionsWidget->SetVisibility(ESlateVisibility::Visible);
			if (ATopDownPlayerController* PlayerController = Cast<ATopDownPlayerController>(GetOwningPlayerController()))
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
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WidgetControllerParams);
	}

	return OverlayWidgetController;
}

void AGameHud::OnRoundChanged(int32 RoundNumber)
{
	// GameHudWidget->UpdateRoundNumber(RoundNumber);
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
