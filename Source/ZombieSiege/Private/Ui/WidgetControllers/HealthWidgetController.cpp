// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/WidgetControllers/HealthWidgetController.h"

#include "Health/PlayerHealthComponent.h"
#include "Player/PlayerCharacter.h"

void UHealthWidgetController::BindCallbackToDependencies()
{
	if (const IHealthComponentInterface* HealthInterface = PlayerController->GetPawn<APlayerCharacter>())
	{
		UHealthComponent* HealthComponent = HealthInterface->GetHealthComponent_Implementation();;
		HealthComponent->OnHealthPercentageChangedEvent.AddLambda([this](const float NewHealthPercentage)
		{
			OnHealthPercentageChangedEvent.Broadcast(NewHealthPercentage);
		});
	}
}

void UHealthWidgetController::BroadcastInitialValues()
{
	if (const IHealthComponentInterface* HealthInterface = PlayerController->GetPawn<APlayerCharacter>())
	{
		const UHealthComponent* HealthComponent = HealthInterface->GetHealthComponent_Implementation();
		OnHealthPercentageChangedEvent.Broadcast(HealthComponent->GetHealthPercentage());
	}
}
