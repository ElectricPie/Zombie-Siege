// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

class UMoneyStoreComponent;
class ARoom;
class UNavModifierComponent;
class UNavLinkComponent;
class UInteractableComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDoorOpenedSignature);

UCLASS()
class ADoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

public:
	UPROPERTY(BlueprintAssignable)
	FOnDoorOpenedSignature OnDoorOpenedEvent;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<AActor> DoorPart;

protected:
	virtual void BeginPlay() override;
	
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
	
	void TryBuyDoor_Server(UMoneyStoreComponent* MoneyStore);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastDoorOpened();
};
