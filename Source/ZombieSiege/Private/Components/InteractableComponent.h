// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "InteractableComponent.generated.h"

class APlayerCharacter;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnInteractSignature, APlayerCharacter* /*InteractingPlayer*/);

/**
 * 
 */
UCLASS(meta=(BlueprintSpawnableComponent))
class UInteractableComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	UInteractableComponent();

	void Interact(APlayerCharacter* InteractingPlayer);

	FOnInteractSignature OnInteractEvent;

private:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
