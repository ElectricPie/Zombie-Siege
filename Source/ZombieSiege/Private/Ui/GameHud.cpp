// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/GameHud.h"

#include "Blueprint/UserWidget.h"
#include "Player/PlayerCharacter.h"
#include "Player/TopDownPlayerController.h"
#include "States/DefenceGameState.h"
#include "Widgets/GameHudWidget.h"

void AGameHud::BeginPlay()
{
	Super::BeginPlay();

	if (GameHudWidgetClass)
	{
		if (ATopDownPlayerController* PlayerController = Cast<ATopDownPlayerController>(GetOwningPlayerController()))
		{
			GameHudWidget = CreateWidget<UGameHudWidget>(PlayerController, GameHudWidgetClass);
			GameHudWidget->AddToViewport();
			Widgets.Add(GameHudWidget);
			if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetPawn()))
			{
				GameHudWidget->Setup(PlayerController, PlayerCharacter);
			}
		}
	}

	if (MenuWidgetClass)
	{
		MenuWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), MenuWidgetClass);
		MenuWidget->AddToViewport();
		MenuWidget->SetVisibility(ESlateVisibility::Collapsed);
		Widgets.Add(MenuWidget);
	}

	if (ADefenceGameState* GameState = GetWorld()->GetGameState<ADefenceGameState>())
	{
		GameState->OnRoundChangedEvent.AddUObject(this, &AGameHud::OnRoundChanged);
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

void AGameHud::ShowGameOver()
{
	GameHudWidget->SetVisibility(ESlateVisibility::Collapsed);

	if (GameOverWidgetClass)
	{
		GameOverWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), GameOverWidgetClass);
		GameOverWidget->AddToViewport();
	}
}

void AGameHud::SwitchActiveWidget(const EGameWidget WidgetToActivate)
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
		case Hud:
			if (GameHudWidget)
			{
				GameHudWidget->SetVisibility(ESlateVisibility::Visible);
			}
			break;
		case Menu:
			if (MenuWidget)
			{
				MenuWidget->SetVisibility(ESlateVisibility::Visible);
			}
			break;
		default: ;
	}
}

void AGameHud::OnRoundChanged(int32 RoundNumber)
{
	GameHudWidget->UpdateRoundNumber(RoundNumber);
}
