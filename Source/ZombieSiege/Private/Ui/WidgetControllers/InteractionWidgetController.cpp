// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/WidgetControllers/InteractionWidgetController.h"

#include "Interactions/InteractableComponent.h"
#include "Interactions/InteractorComponent.h"
#include "Player/PlayerCharacter.h"

void UInteractionWidgetController::BindCallbackToDependencies()
{
	Super::BindCallbackToDependencies();

	if (const APlayerCharacter* PlayerCharacter = PlayerController->GetPawn<APlayerCharacter>())
	{
		UInteractorComponent* InteractorComponent = PlayerCharacter->GetInteractorComponent();
		InteractorComponent->OnEnterInteractableEvent.AddLambda([this](const UInteractableComponent* InteractableComponent)
		{
			InteractionMessageEvent.Broadcast(true, InteractableComponent->GetInteractMessage());
		});
		InteractorComponent->OnExitInteractableEvent.AddLambda([this](const UInteractableComponent* InteractableComponent)
		{
			InteractionMessageEvent.Broadcast(false, FText::GetEmpty());
		});
	}
}
