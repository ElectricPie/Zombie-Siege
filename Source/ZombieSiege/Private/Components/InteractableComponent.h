// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "InteractableComponent.generated.h"

class UInteractorComponent;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnInteractSignature, TWeakObjectPtr<AController> /*InteractionInstigator*/, TWeakObjectPtr<AActor> /*InteractionCauser*/);

/**
 * 
 */
UCLASS(meta=(BlueprintSpawnableComponent))
class UInteractableComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	UInteractableComponent();

	void Interact(TWeakObjectPtr<AController> InteractionInstigator, TWeakObjectPtr<AActor> InteractionCauser);

	FOnInteractSignature OnInteractEvent;

	void SetInteractMessage(const FText NewInteractMessage) { InteractMessage = NewInteractMessage; }
	FText GetInteractMessage() { return InteractMessage; }
	void SetCanInteract(bool bNewCanInteract);
	
private:
	UPROPERTY(EditAnywhere)
	FText InteractMessage = FText::FromString("Interact");
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	TArray<TWeakObjectPtr<UInteractorComponent>> InteractorsInRange;

	UPROPERTY(EditAnywhere, Category="Interaction")
	bool bCanInteract;
};
