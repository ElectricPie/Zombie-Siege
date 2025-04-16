// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactions/InteractableComponent.h"

void UInteractableComponent::TryInteract(AController* InteractionInstigator, AActor* InteractionCauser)
{
	if (!bCanInteract)
		return;
	
	OnInteractEvent.Broadcast(InteractionInstigator, InteractionCauser);
}

void UInteractableComponent::SetCanInteract(const bool bNewCanInteract)
{
	bCanInteract = bNewCanInteract;
}

void UInteractableComponent::InteractionSuccessful()
{
	OnInteractionSuccessfulEvent.Broadcast(true, this);
}

void UInteractableComponent::ConsumeInteractable() const
{
	if (bCanBeConsumed)
	{
		OnInteractableConsumedEvent.Broadcast();
	}
}
