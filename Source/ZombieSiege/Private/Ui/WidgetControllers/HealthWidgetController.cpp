// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/WidgetControllers/HealthWidgetController.h"

#include "Components/HealthComponent.h"
#include "Player/PlayerCharacter.h"

void UHealthWidgetController::BindCallbackToDependencies()
{
	if (const APlayerCharacter* PlayerCharacter = PlayerController->GetPawn<APlayerCharacter>())
	{
		UHealthComponent* HealthComponent = PlayerCharacter->GetHealthComponent();
		HealthComponent->OnHealthPercentageChangedEvent.AddLambda([this](const float NewHealthPercentage)
		{
			OnHealthPercentageChangedEvent.Broadcast(NewHealthPercentage);
		});
	}
}

void UHealthWidgetController::BroadcastInitialValues()
{
	if (const APlayerCharacter* PlayerCharacter = PlayerController->GetPawn<APlayerCharacter>())
	{
		const UHealthComponent* HealthComponent = PlayerCharacter->GetHealthComponent();
		OnHealthPercentageChangedEvent.Broadcast(HealthComponent->GetHealthPercentage());
	}
}
