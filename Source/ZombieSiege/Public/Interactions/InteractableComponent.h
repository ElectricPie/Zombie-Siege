// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "InteractableComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnInteractSignature, AController* /*InteractionInstigator*/, AActor* /*InteractionCauser*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnInteractionSuccessfulSignature, const bool /*bWasSuccessful*/, UInteractableComponent* /*SuccessfulInteractableComponent*/);
DECLARE_MULTICAST_DELEGATE(FOnInteractableConsumedSignature);

/**
 * 
 */
UCLASS(meta=(BlueprintSpawnableComponent))
class UInteractableComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	FOnInteractSignature OnInteractEvent;
	FOnInteractionSuccessfulSignature OnInteractionSuccessfulEvent;
	FOnInteractableConsumedSignature OnInteractableConsumedEvent;
	
public:
	void TryInteract(AController* InteractionInstigator, AActor* InteractionCauser);
	
	void SetInteractMessage(const FText& NewInteractMessage) { InteractMessage = NewInteractMessage; }
	FText GetInteractMessage() const { return InteractMessage; }
	void SetCanInteract(bool bNewCanInteract);
	bool GetCanInteract() const { return bCanInteract; }

	void InteractionSuccessful();

	void ConsumeInteractable() const;
	
private:
	UPROPERTY(EditAnywhere)
	FText InteractMessage = FText::FromString("Interact");

	UPROPERTY(EditAnywhere, Category="Interaction")
	bool bCanInteract;
	UPROPERTY(EditAnywhere, Category="Interaction", meta=(ToolTip="Indicates if the interactable can be consumed after interaction"))
	bool bCanBeConsumed = false;
};
