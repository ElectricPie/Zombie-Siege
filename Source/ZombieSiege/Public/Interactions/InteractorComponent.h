// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractorComponent.generated.h"

class UInteractableComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInteractableChangedSignature, const UInteractableComponent* /*InteractableComponent*/)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnInteratorSuccessfulSignature, bool /*bSuccess*/);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UInteractorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FOnInteractableChangedSignature OnEnterInteractableEvent;
	FOnInteractableChangedSignature OnExitInteractableEvent;
	
public:	
	// Sets default values for this component's properties
	UInteractorComponent();

	void OnOverlapBegin(const AActor* OtherActor);
	void OnOverlapEnd(const AActor* OtherActor);

	void Interact();
	
private:
	UPROPERTY(EditAnywhere, meta=(ClampMin=0.f, UIMin=0.f, ToolTip="The amout of time to wait before the interaction is considered failed"))
	float InteractionTimeout = 0.5f;
	
	TWeakObjectPtr<UInteractableComponent> CurrentInteractable;
	FDelegateHandle InteractionSuccessfulHandle;
	FTimerHandle TimeoutHandle;
	
private:
	UFUNCTION(Server, Reliable)
	void ServerInteract(UInteractableComponent* InteractableComponent);
	UFUNCTION(Client, Reliable)
	void ClientInteractionSuccessful(const bool bSuccess, UInteractableComponent* InteractableComponent);
};
