// Fill out your copyright notice in the Description page of Project Settings.

#include "Rooms/Door.h"

#include "Components/InteractableComponent.h"
#include "Components/MoneyStoreComponent.h"

#define DEFAULT_DOOR_INTERACT_MESSAGE "Open Door"

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = BaseComponent;

	DoorFrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Frame"));
	DoorFrameMesh->SetupAttachment(RootComponent);
	
	InteractableTrigger = CreateDefaultSubobject<UInteractableComponent>(TEXT("Interactable Trigger"));
	InteractableTrigger->SetupAttachment(RootComponent);
	InteractableTrigger->OnInteractEvent.AddUObject(this, &ADoor::OnInteract);
	InteractableTrigger->SetInteractMessage(FText::FromString(TEXT(DEFAULT_DOOR_INTERACT_MESSAGE)));
	InteractableTrigger->SetCanInteract(true);
}

void ADoor::OnInteract(TWeakObjectPtr<AController> InteractionInstigator, TWeakObjectPtr<AActor> InteractionCauser)
{
	if (DoorPart == nullptr) return;
	if (UMoneyStoreComponent* MoneyStore = InteractionInstigator->GetComponentByClass<UMoneyStoreComponent>())
	{
		if (MoneyStore->TakeMoney(OpenCost))
		{
			DoorPart->Destroy();
			InteractableTrigger->SetCanInteract(false);
		}
	}
}