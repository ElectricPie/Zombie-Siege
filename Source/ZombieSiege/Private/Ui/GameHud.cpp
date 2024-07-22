// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/GameHud.h"

#include "Blueprint/UserWidget.h"
#include "GameModes/ZombieDefenceGameMode.h"
#include "Player/PlayerCharacter.h"
#include "Player/TopDownPlayerController.h"
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
			if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetPawn()))
			{
				GameHudWidget->Setup(PlayerController, PlayerCharacter);
			}
		}
	}

	if (AZombieDefenceGameMode* GameMode = GetWorld()->GetAuthGameMode<AZombieDefenceGameMode>())
	{
		GameMode->OnRoundChangedEvent.AddUObject(this, &AGameHud::OnRoundChanged);
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

void AGameHud::OnRoundChanged(int32 RoundNumber)
{
	GameHudWidget->UpdateRoundNumber(RoundNumber);
}

