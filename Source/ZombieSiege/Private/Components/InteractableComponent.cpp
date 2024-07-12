// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InteractableComponent.h"

#include "Player/PlayerCharacter.h"

UInteractableComponent::UInteractableComponent()
{
	OnComponentBeginOverlap.AddDynamic(this, &UInteractableComponent::OnOverlapBegin);
	OnComponentEndOverlap.AddDynamic(this, &UInteractableComponent::OnOverlapEnd);
}

void UInteractableComponent::Interact(APlayerCharacter* InteractingPlayer)
{
	OnInteractEvent.Broadcast(InteractingPlayer);
}

void UInteractableComponent::SetDisplayMessage(const bool bShouldDisplayMessage)
{
	bDisplayMessage = bShouldDisplayMessage;

	for (const auto& Player : PlayersInRange)
	{
		if (bDisplayMessage)
		{
			Player->AddInteractable(this);
		}
		else
		{
			Player->RemoveInteractable(this);
		}
	}
}

void UInteractableComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                            const FHitResult& SweepResult)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor))
	{
		PlayersInRange.Add(PlayerCharacter);
		if (bDisplayMessage)
		{
			PlayerCharacter->AddInteractable(this);
		}
	}
}

void UInteractableComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor))
	{
		PlayersInRange.Remove(PlayerCharacter);
		PlayerCharacter->RemoveInteractable(this);
	}
}
