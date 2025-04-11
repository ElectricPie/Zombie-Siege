// Fill out your copyright notice in the Description page of Project Settings.

#include "Rooms/Door.h"
#include "Rooms/Room.h"
#include "Interactions/InteractableComponent.h"
#include "Components/MoneyStoreComponent.h"
#include "Money/MoneyStoreInterface.h"

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
	InteractableTrigger->SetInteractMessage(FText::FromString(DEFAULT_DOOR_INTERACT_MESSAGE));
	InteractableTrigger->SetCanInteract(true);
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();

	InteractableTrigger->OnInteractEvent.AddLambda(
		[this](const AController* InteractionInstigator, const AActor* InteractionCauser)
		{
			if (UMoneyStoreComponent* MoneyStoreComponent = IMoneyStoreInterface::Execute_GetMoneyStoreComponent(
				InteractionInstigator))
			{
				BuyDoor(MoneyStoreComponent);
			}
		});

	const FText InteractMessage = FText::FromString(FString::Printf(TEXT("Open Door [Costs %d]"), OpenCost));
	InteractableTrigger->SetInteractMessage(InteractMessage);
}

void ADoor::BuyDoor(UMoneyStoreComponent* MoneyStore)
{
	checkf(DoorPart, TEXT("DoorPart is not set on Door actor %s"), *GetName());

	// Not enough money
	if (!MoneyStore->TakeMoney(OpenCost))
		return;
	
	DoorPart->Destroy();
	InteractableTrigger->SetCanInteract(false);
	OnDoorOpenedEvent.Broadcast();

	for (const auto& Room : ConnectedRooms)
	{
		Room->UnlockRoom();
	}
}
