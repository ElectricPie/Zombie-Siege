// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

class ARoom;
class UInteractableComponent;
class UMoneyStoreComponent;
class UNavModifierComponent;
class UNavLinkComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDoorOpenedSignature);

UCLASS()
class ADoor : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere, Category="Components")
	USceneComponent* BaseComponent;
	UPROPERTY(VisibleAnywhere, Category="Components")
	UStaticMeshComponent* DoorFrameMesh;
	UPROPERTY(VisibleAnywhere, Category="Components")
	UInteractableComponent* InteractableTrigger;

	UPROPERTY(EditAnywhere, Category="Money", meta=(ClampMin=0, UIMin=0))
	int32 OpenCost = 500;

	UPROPERTY(EditAnywhere, Category="Room")
	TArray<TWeakObjectPtr<ARoom>> ConnectedRooms;

public:	
	// Sets default values for this actor's properties
	ADoor();
	
protected:
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintImplementableEvent)
	void OpenDoor();
	
private:
	void TryBuyDoor_Server(UMoneyStoreComponent* MoneyStore);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastDoorOpened();
};
