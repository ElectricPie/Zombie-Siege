// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InteractableComponent.h"

#include "InteractorComponent.h"

UInteractableComponent::UInteractableComponent()
{
	OnComponentBeginOverlap.AddDynamic(this, &UInteractableComponent::OnOverlapBegin);
	OnComponentEndOverlap.AddDynamic(this, &UInteractableComponent::OnOverlapEnd);
}

void UInteractableComponent::Interact(TWeakObjectPtr<AController> InteractionInstigator, TWeakObjectPtr<AActor> InteractionCauser)
{
	if (!bCanInteract) return;
	
	OnInteractEvent.Broadcast(InteractionInstigator, InteractionCauser);
}

void UInteractableComponent::SetCanInteract(const bool bNewCanInteract)
{
	bCanInteract = bNewCanInteract;

	for (const auto& Interactor : InteractorsInRange)
	{
		if (bCanInteract)
		{
			Interactor->AddInteractable(this);
		}
		else
		{
			Interactor->RemoveInteractable(this);
		}
	}
}

void UInteractableComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                            const FHitResult& SweepResult)
{
	if (OtherActor == nullptr) return;
	
	if (UInteractorComponent* InteractorComponent = OtherActor->GetComponentByClass<UInteractorComponent>())
	{
		InteractorsInRange.Add(InteractorComponent);
		if (bCanInteract)
		{
			InteractorComponent->AddInteractable(this);
		}
	}
}

void UInteractableComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == nullptr) return;
	
	if (UInteractorComponent* InteractorComponent = OtherActor->GetComponentByClass<UInteractorComponent>())
	{
		InteractorsInRange.Remove(InteractorComponent);
		InteractorComponent->RemoveInteractable(this);
	}
}
