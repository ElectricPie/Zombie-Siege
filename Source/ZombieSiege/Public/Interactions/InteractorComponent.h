// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractorComponent.generated.h"

class UInteractableComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInteractableChangedSignature, UInteractableComponent* /*InteractableComponent*/)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UInteractorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractorComponent();

	FOnInteractableChangedSignature OnEnterInteractableEvent;
	FOnInteractableChangedSignature OnExitInteractableEvent;

	UFUNCTION(Server, Reliable)
	void ServerInteract();
	
	void AddInteractable(UInteractableComponent* InteractableComponent);
	void RemoveInteractable(const UInteractableComponent* InteractableComponent);

private:
	TWeakObjectPtr<UInteractableComponent> CurrentInteractable;

};
