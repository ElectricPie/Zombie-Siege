// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactions/InteractableComponent.h"

#include "Net/UnrealNetwork.h"

UInteractableComponent::UInteractableComponent()
{
	SetIsReplicatedByDefault(true);
}

void UInteractableComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInteractableComponent, bCanInteract);
}

void UInteractableComponent::TryInteract(AController* InteractionInstigator, APawn* InteractionCauser)
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
