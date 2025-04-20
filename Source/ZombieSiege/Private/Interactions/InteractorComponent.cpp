// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactions/InteractorComponent.h"
#include "Interactions/InteractableComponent.h"

// Sets default values for this component's properties
UInteractorComponent::UInteractorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UInteractorComponent::ServerInteract_Implementation(UInteractableComponent* InteractableComponent)
{
	if (CurrentInteractable == nullptr || InteractionSuccessfulHandle.IsValid())
		return;

	InteractionSuccessfulHandle = CurrentInteractable->OnInteractionSuccessfulEvent.AddLambda(
	[this](const bool bWasSuccessful, UInteractableComponent* SuccessfulInteractableComponent)
	{
		ClientInteractionSuccessful(bWasSuccessful, SuccessfulInteractableComponent);
		InteractionSuccessfulHandle.Reset();
		TimeoutHandle.Invalidate();
	});
	
	GetWorld()->GetTimerManager().SetTimer(TimeoutHandle, FTimerDelegate::CreateLambda([this]()
	{
		InteractionSuccessfulHandle.Reset();
		TimeoutHandle.Invalidate();
	}), InteractionTimeout, false);
	
	CurrentInteractable->TryInteract(GetOwner()->GetInstigatorController(), GetOwner<APawn>());
}

void UInteractorComponent::OnOverlapBegin(const AActor* OtherActor)
{
	if (UInteractableComponent* InteractableComponent = OtherActor->GetComponentByClass<UInteractableComponent>())
	{
		if (InteractableComponent->GetCanInteract())
		{
			CurrentInteractable = InteractableComponent;
			OnEnterInteractableEvent.Broadcast(InteractableComponent);
		}
	}
}

void UInteractorComponent::OnOverlapEnd(const AActor* OtherActor)
{
	if (const UInteractableComponent* InteractableComponent = OtherActor->GetComponentByClass<UInteractableComponent>())
	{
		if (CurrentInteractable == InteractableComponent)
		{
			CurrentInteractable = nullptr;
			OnExitInteractableEvent.Broadcast(InteractableComponent);
		}
	} 
}

void UInteractorComponent::Interact()
{
	ServerInteract(CurrentInteractable.Get());
}

void UInteractorComponent::ClientInteractionSuccessful_Implementation(const bool bSuccess, UInteractableComponent* InteractableComponent)
{
	if (bSuccess)
	{
		InteractableComponent->ConsumeInteractable();
	}

	CurrentInteractable = nullptr;
	OnExitInteractableEvent.Broadcast(InteractableComponent);
}

