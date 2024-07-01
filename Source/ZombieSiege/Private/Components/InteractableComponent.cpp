// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InteractableComponent.h"

#include "Player/PlayerCharacter.h"

UInteractableComponent::UInteractableComponent()
{
	OnComponentBeginOverlap.AddDynamic(this, &UInteractableComponent::OnOverlapBegin);
	OnComponentEndOverlap.AddDynamic(this, &UInteractableComponent::OnOverlapEnd);
}

void UInteractableComponent::Interact()
{
	OnInteractEvent.Broadcast();
}

void UInteractableComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor))
	{
		PlayerCharacter->AddInteractable(this);
	}
}

void UInteractableComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor))
	{
		PlayerCharacter->RemoveInteractable(this);
	}
}
