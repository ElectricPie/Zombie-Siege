// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InteractorComponent.h"
#include "Components/InteractableComponent.h"

// Sets default values for this component's properties
UInteractorComponent::UInteractorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UInteractorComponent::Interact()
{
	if (CurrentInteractable == nullptr) return;

	CurrentInteractable->Interact(GetOwner()->GetInstigatorController(), GetOwner());
}

void UInteractorComponent::AddInteractable(UInteractableComponent* InteractableComponent)
{
	if (InteractableComponent == nullptr) return;
	
	CurrentInteractable = InteractableComponent;
	OnEnterInteractableEvent.Broadcast(CurrentInteractable);
}

void UInteractorComponent::RemoveInteractable(const UInteractableComponent* InteractableComponent)
{
	if (InteractableComponent == nullptr) return;
	// Don't want to remove the current one if its not the one we are leaving
	if (CurrentInteractable != InteractableComponent) return;

	OnExitInteractableEvent.Broadcast(CurrentInteractable);
	CurrentInteractable = nullptr;
}
