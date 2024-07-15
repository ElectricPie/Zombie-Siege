// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

class UNavModifierComponent;
class UNavLinkComponent;
class UInteractableComponent;

UCLASS()
class ADoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<AActor> DoorPart;

private:
	UPROPERTY(VisibleAnywhere, Category="Components")
	USceneComponent* BaseComponent;
	UPROPERTY(VisibleAnywhere, Category="Components")
	UStaticMeshComponent* DoorFrameMesh;
	UPROPERTY(VisibleAnywhere, Category="Components")
	UInteractableComponent* InteractableTrigger;
	
	UFUNCTION()
	void OnInteract(APlayerCharacter* InteractingPlayer);
};
